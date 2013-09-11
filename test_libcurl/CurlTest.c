#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int main(void)
{
	CURL *curl;
	CURLcode res;

	static const char *content= "nav_id=1&nav_etime=2012-07-18%2015:00:03";

	char url[200];

	curl = curl_easy_init();
	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, "http://edss.isima.fr/sites/smir/sees/gate/navupdate?nav_id=1&nav_etime=2012-07-18%2015:00:03");
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);

		/* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
		   itself */ 
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(content));

		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

//		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, url);
//		printf("%s\n", url);

		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}
	return 0;
}
