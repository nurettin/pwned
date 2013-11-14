#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define FROM    "<onurtugcu@gmail.com>"
#define TO      "<onurtugcu@gmail.com>"
#define CC      "<onurtugcu@gmail.com>"

static const char* payload_text[]={
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\n",
  "To: " TO "\n",
  "From: " FROM "(Example User)\n",
  "Cc: " CC "(Another example User)\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>\n",
  "Subject: SMTP TLS example message\n",
  "\n", /* empty line to divide headers from body, see RFC5322 */
  "The body of the message starts here.\n",
  "\n",
  "It could be a lot of lines, could be MIME encoded, whatever.\n",
  "Check RFC5322.\n",
  NULL
};

std::string smtp_header(
  std::string const &to
, std::string const &from
, std::vector<std::string> const &cc
, std::string const &subject
, std::string const &body)
{
  std::ostringstream out;
  return out.str();
}

struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;

  if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = payload_text[upload_ctx->lines_read];

  if (data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read ++;
    return len;
  }
  return 0;
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx;

  upload_ctx.lines_read = 0;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USERNAME, "zopsmtp@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "4r3e2w1q");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    /* value for envelope reverse-path */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);
    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
    recipients = curl_slist_append(recipients, TO);
    recipients = curl_slist_append(recipients, CC);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    /* In this case, we're using a callback function to specify the data. You
     * could just use the CURLOPT_READDATA option to specify a FILE pointer to
     * read from.
     */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
  }
  return 0;
}
