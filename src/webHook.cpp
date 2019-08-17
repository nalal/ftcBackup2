#include <iostream>
#include <string>
#include <curl/curl.h>
#include <future> 
#include <thread>
#include <chrono>
#include "webHook.h"

using namespace std;

bool noLog = true;

bool verbose = false;

const char* link;

void enableLog()
{
	noLog = false;
}

void curlVerbose()
{
	verbose = true;
}

void setLink(const char* linkIn)
{
	link = linkIn;
}

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

void relayMsg(const char *s)
{
	if(!noLog)
	{
		relayMsgRunner(s);
	}
}
