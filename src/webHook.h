#ifndef WEBHOOK_HPP_INLCUDE
#define WEBHOOK_HPP_INCLUDE

// Send message to webhook
void relayMsg(const char *s);
// Enable webhook
void enableLog();
// Set webhook link
void setLink(const char* linkIn);
// Set curl to output to console
void curlVerbose();

#endif
