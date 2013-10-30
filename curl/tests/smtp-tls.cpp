#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

void smtp_header(std::string const &from, std::vector<std::string> const &to, std::vector<std::string> const &cc
  , std::string const &subject, std::string const &body, std::stringstream &io)
{
  char a[42];
  std::time_t current_time;
  std::time(&current_time);
  std::tm *timeinfo = std::localtime(&current_time);
  std::strftime(a, 42, "%c %z", timeinfo);
  
  io<< "Date: "<< a<< "\nTo: ";
  for(auto &t: to) io<< t<< ' ';
  io<< "\nFrom: "<< from<< '\n';
  for(auto &c: cc) io<< c<< ' ';
  io<< "\nSubject: "<< subject;
  io<< "\nBody: "<< body<< '\n';
}

std::size_t write(void* ptr, std::size_t size, std::size_t nmemb, void* userp)
{
  std::size_t len= size* nmemb;
  if(size== 0 || nmemb== 0|| len< 1)
    return 0;

  std::stringstream* stream= static_cast<std::stringstream*>(userp);
  bool ok= stream-> read(ptr, len);
  if(!ok) return 0;
  return len;
}

int main()
{
  CURL *curl;
  CURLcode res;
  curl_slist *recipients = NULL;
  curl = curl_easy_init();
  if(!curl) return EXIT_FAILURE;

  curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
  curl_easy_setopt(curl, CURLOPT_USERNAME, "zopsmtp@gmail.com");
  curl_easy_setopt(curl, CURLOPT_PASSWORD, "4r3e2w1q");
  curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
  curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);
  
  recipients = curl_slist_append(recipients, "onurtugcu@gmail.com");
  recipients = curl_slist_append(recipients, "onurtugcu@gmail.com");
  curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

  std::stringstream stream;
  smtp_header("onurtugcu@gmail.com", { "onurtugcu@gmail.com" }, { "onurtugcu@gmail.com" }, "Konu", "Icerik", stream);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, write);
  curl_easy_setopt(curl, CURLOPT_READDATA, &stream);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  res = curl_easy_perform(curl);
  if(res != CURLE_OK)
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));

  curl_slist_free_all(recipients);
  curl_easy_cleanup(curl);
}
