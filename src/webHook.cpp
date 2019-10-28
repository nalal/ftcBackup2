// C++ libs
#include <iostream>
#include <string>
#include <future> 
#include <thread>
#include <chrono>

// Other libs
#include <curl/curl.h>

// Local header
#include "webHook.h"

using namespace std;

bool noLog = true;

bool verbose = false;

const char* link;

// Make output for webhook enabled
void enableLog()
{
	noLog = false;
}

// Make curl output verbose
void curlVerbose()
{
	verbose = true;
}

// Set webhook URL
void setLink(const char* linkIn)
{
	link = linkIn;
}

// Send message to webhook
void relayMsgRunner(const char *s)
{
	CURL *curl;
	string outPut = s;
	outPut = "`[SYSTEM@FTC-001]`: " + outPut;
	outPut = "{\"content\": \"" + outPut + "\"} ";
	if(verbose)
	{
		cout << outPut << endl;
	}
	if(link!=NULL && verbose)
	{
		cout << "link is " << link << endl;
	}
	curl = curl_easy_init();
	struct curl_slist *headers = NULL;
	curl_slist_append(headers, "Accept: application/json");
	curl_slist_append(headers, "Content-Type: application/json");
	curl_slist_append(headers, "charsets: utf-8");
	if(curl) 
	{
		//Can't do nothin about line limit on this one ¯\_(ツ)_/¯
		//curl_easy_setopt(curl, CURLOPT_URL, 
		curl_easy_setopt(curl, CURLOPT_URL, link);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, outPut.length());
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "tlsv1_2");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, outPut.c_str());
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

// Check if messages are to be sent
void relayMsg(const char *s)
{
	if(!noLog)
	{
		relayMsgRunner(s);
	}
}
