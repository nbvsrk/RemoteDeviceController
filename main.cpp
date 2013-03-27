/* 
 * File:   main.cpp
 * Author: satya gowtham kudupudi
 *
 * Created on 15 March, 2013, 12:18 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <libxml/xpathInternals.h>
#include <getopt.h>
#include <libxml/xmlstring.h>
#include <fstream>
#include <sys/stat.h> 
#include <fcntl.h>
#include <openssl/md5.h>
#include <time.h>
#include "myconverters.h"
#include "mystdlib.h"
#include "myxml.h"
#include "mycurl.h"
#include <signal.h>
#include <sys/wait.h>

using namespace std;

/*
 * 
 */
sig_atomic_t child_exit_status;

void print_usage(FILE* stream, int exit_code, char* program_name) {
    fprintf(stream, "Usage: %s options [ inputfile ...]\n", program_name);
    fprintf(stream, "-h --help Display this usage information.\n-r --run run client.\n-i --install print install MECCamController.\n");
    exit(exit_code);
}

void clean_up_child_process(int signal_number) {
    int status;
    wait(&status);
    child_exit_status = status;
}

void run() {
    int ecode;
    while (true) {
	printf("Checking server status:\n");
	string response = httpReq("183.82.98.147", "11000", "GET", "/RestService/RestServiceImpl.svc/xml/", "text/xml", "", false);
	cout << response;
	xmlInitParser();
	xmlChar *res = (xmlChar*) response.c_str();
	xmlDoc *xd = xmlParseDoc(res);
	//xmlSaveFormatFileEnc( "output.xml", xd, "utf-8", 1 );
	xmlXPathContext *xpathCtx = xmlXPathNewContext(xd);
	xmlXPathRegisterNs(xpathCtx, (xmlChar*) "n", (xmlChar*) "http://tempuri.org/");
	xmlXPathObject * xpathObj = xmlXPathEvalExpression((xmlChar*) "/n:XMLDataResponse/n:XMLDataResult", xpathCtx);
	xmlNode *node = xpathObj->nodesetval->nodeTab[0];
	//print_xpath_nodes(xpathObj->nodesetval, stdout);
	//printf("%s\n", (char*) xmlNodeGetContent(node));
	//xmlNode *root = xmlDocGetRootElement(xd);
	//std::cout << "The root node is";

	//print_node(node);
	pid_t fcpid;
	time_t rawtime;
	struct tm * timeinfo;
	char fn [80];
	time(&rawtime);

	if (strcmp((char*) xmlNodeGetContent(node), "S") == 0) {
	    //if (true) {
	    //while (true) {
	    timeinfo = localtime(&rawtime);
	    strftime(fn, 80, "record/%Y-%m-%d %H:%M:%S.flv", timeinfo);
	    printf("Starting webcam streaming...\n");
	    char * args[] = {"ffmpeg", "-f", "v4l2", "-r", "15", "-s", "40x30", "-i", "/dev/video0", "-f", "flv", "rtmp://192.168.2.25:25333/venkat/test1", NULL};
	    //ecode = system("pkill -9 ffmpeg &> /dev/null");
	    sleep(3);
	    fcpid = spawn("ffmpeg", args);
	    printf("Webcam streaming started");
	    fflush(stdout);
	    //sleep(15 * 60);
	    //}
	} else if (strcmp((char*) xmlNodeGetContent(node), "R") == 0) {
	    timeinfo = localtime(&rawtime);
	    strftime(fn, 80, "record/%Y-%m-%d %H:%M:%S.flv", timeinfo);
	    ecode = system("pkill -9 ffmpeg &> /dev/null");
	    char * arg[] = {"ffmpeg", "-f", "v4l2", "-r", "15", "-s", "40x30", "-i", "/dev/video0", "-f", "flv", fn, NULL};
	    fcpid = spawn("ffmpeg", arg);
	} else if (strcmp((char*) xmlNodeGetContent(node), "N") == 0) {
	    printf("Stopping webcam streaming..\n");
	    ecode = system("pkill -9 ffmpeg &> /dev/null");
	    printf("Webcam stream stopped\n");
	}
	sleep(3);
    }
}

void install() {
    string username;
    cout << "Installation process for MEKCamController\nPlease give in prompted information...";
    cout << "\nusername: ";
    char ch;
    ch = getch();
    while ((int) ch != 10) {
	if (ch == '\b') {
	    if (username.size() > 0) {
		cout << "\b \b";
		fflush(stdout);
		username.erase(username.size() - 1);
	    }
	} else {
	    username.push_back(ch);
	    cout << ch;
	}
	ch = getch();
    }
    string password;
    cout << "\npassword: ";
    fflush(stdout);
    fflush(stdin);
    ch = getch();
    while ((int) ch != 10) {
	if (ch == '\b') {
	    cout << "\b \b";
	    fflush(stdout);
	    password.erase(password.size() - 1);
	} else {
	    password.push_back(ch);
	    cout << '*';
	}
	ch = getch();
    }
    xmlInitParser();
    xmlDoc* xd = xmlParseFile("VendorLoginForClientComponent.xml");
    xmlXPathContext *xpathCtx = xmlXPathNewContext(xd);
    xmlXPathRegisterNs(xpathCtx, (xmlChar*) "soap12", (xmlChar*) "http://www.w3.org/2003/05/soap-envelope");
    xmlXPathRegisterNs(xpathCtx, (xmlChar*) "xsd", (xmlChar*) "http://www.w3.org/2001/XMLSchema");
    xmlXPathRegisterNs(xpathCtx, (xmlChar*) "xsi", (xmlChar*) "http://www.w3.org/2001/XMLSchema-instance");
    xmlXPathRegisterNs(xpathCtx, (xmlChar*) "n", (xmlChar*) "EAKSuperVisionService");
    xmlXPathObject * accNameXpathObj = xmlXPathEvalExpression((xmlChar*) "/soap12:Envelope/soap12:Body/n:VendorLoginForClientComponent/n:AccountName", xpathCtx);
    xmlXPathObject * usrNameXpathObj = xmlXPathEvalExpression((xmlChar*) "/soap12:Envelope/soap12:Body/n:VendorLoginForClientComponent/n:Username", xpathCtx);
    xmlXPathObject * passXpathObj = xmlXPathEvalExpression((xmlChar*) "/soap12:Envelope/soap12:Body/n:VendorLoginForClientComponent/n:Password", xpathCtx);

    xmlNode *node = accNameXpathObj->nodesetval->nodeTab[0];
    xmlNodeSetContent(node, (xmlChar*) "");
    xmlNode *node2 = usrNameXpathObj->nodesetval->nodeTab[0];
    xmlNodeSetContent(node2, (xmlChar*) username.c_str());
    xmlNode *node3 = passXpathObj->nodesetval->nodeTab[0];
    xmlNodeSetContent(node3, (xmlChar*) password.c_str());

    xmlChar* s;
    int size;
    xmlDocDumpMemory(xd, &s, &size);
    xmlCleanupParser();
    xmlInitParser();
    string res = SOAPReq("192.168.2.16", "80", "/barista/CamCaptureService.asmx", "EAKSuperVisionService/VendorLoginForClientComponent", string((char*) s), false);
    xmlDoc* xd2 = xmlParseDoc((xmlChar*) res.c_str());
    xmlXPathContext *xpc2 = xmlXPathNewContext(xd2);
    xmlXPathRegisterNs(xpc2, (xmlChar*) "soap12", (xmlChar*) "http://www.w3.org/2003/05/soap-envelope");
    xmlXPathRegisterNs(xpc2, (xmlChar*) "xsd", (xmlChar*) "http://www.w3.org/2001/XMLSchema");
    xmlXPathRegisterNs(xpc2, (xmlChar*) "xsi", (xmlChar*) "http://www.w3.org/2001/XMLSchema-instance");
    xmlXPathRegisterNs(xpc2, (xmlChar*) "xs", (xmlChar*) "http://www.w3.org/2001/XMLSchema");
    xmlXPathRegisterNs(xpc2, (xmlChar*) "msdata", (xmlChar*) "urn:schemas-microsoft-com:xml-msdata");
    xmlXPathRegisterNs(xpc2, (xmlChar*) "diffgr", (xmlChar*) "urn:schemas-microsoft-com:xml-diffgram-v1");
    //xmlXPathRegisterNs(xpc2, (xmlChar*) "o", (xmlChar*) "");
    xmlXPathRegisterNs(xpc2, (xmlChar*) "n", (xmlChar*) "EAKSuperVisionService");
    xmlXPathObject * tableXpathObj = xmlXPathEvalExpression((xmlChar*) "//*[local-name()='Table']", xpc2);
    xmlNode *node4;
    int nn = tableXpathObj->nodesetval->nodeNr;
    if (nn > 0) {
	cout << "\nSelect one of the systems:\n----------------------------\n";
	cout << "SNO\tBranchId\tBranchName\tSystemId\tSystemName";
	fflush(stdout);
	int i;
	char* bn[nn];
	for (i = 0; i < tableXpathObj->nodesetval->nodeNr; i++) {
	    node4 = tableXpathObj->nodesetval->nodeTab[i];
	    cout << ("\n" + string(itoa(i + 1)) + ".\t");
	    fflush(stdout);
	    cout << (string((char*) xmlNodeGetContent(node4->children)) + "\t\t" + string((char*) xmlNodeGetContent(node4->children->next)) + "\t" + string(bn[i] = (char*) xmlNodeGetContent(node4->children->next->next)) + "\t\t" + string((char*) xmlNodeGetContent(node4->children->next->next->next)));
	    fflush(stdout);
	}
	xmlCleanupParser();
	cout << "\nSelect a system(SNO): ";
	cin >> i;
	if (i > 0 && i <= nn) {
	    i -= 1;
	    /*unsigned char encryptedKey[MD5_DIGEST_LENGTH];
	     *MD5((unsigned char*) &bn[i], strlen(bn[i]), encryptedKey);
	     *char mdString[33];
	     *for (int i = 0; i < 16; i++)
	     *sprintf(&mdString[i * 2], "%02x", (unsigned int) encryptedKey[i]);
	     *cout << bn[i];
	     *cout << mdString;
	     */
	    xmlInitParser();
	    xmlDoc * xusis = xmlParseFile("UpdateSystemInstallStatus.xml");
	    xmlXPathContext * xcusis = xmlXPathNewContext(xusis);
	    xmlXPathRegisterNs(xcusis, (xmlChar*) "soap12", (xmlChar*) "http://www.w3.org/2003/05/soap-envelope");
	    xmlXPathRegisterNs(xcusis, (xmlChar*) "xsd", (xmlChar*) "http://www.w3.org/2001/XMLSchema");
	    xmlXPathRegisterNs(xcusis, (xmlChar*) "xsi", (xmlChar*) "http://www.w3.org/2001/XMLSchema-instance");
	    xmlXPathRegisterNs(xcusis, (xmlChar*) "n", (xmlChar*) "EAKSuperVisionService");
	    xmlXPathObject * skXpathObj = xmlXPathEvalExpression((xmlChar*) "//*[local-name()='SecurityKey']", xcusis);
	    xmlXPathObject * stXpathObj = xmlXPathEvalExpression((xmlChar*) "//*[local-name()='InstallStatus']", xcusis);

	    xmlNode* node4 = skXpathObj->nodesetval->nodeTab[0];
	    xmlNodeSetContent(node4, (xmlChar*) bn[i]);
	    xmlNode* node5 = stXpathObj->nodesetval->nodeTab[0];
	    xmlNodeSetContent(node5, (xmlChar*) "1");

	    xmlDocDumpMemory(xusis, &s, &size);
	    xmlCleanupParser();
	    res = SOAPReq("192.168.2.16", "80", "/barista/CamCaptureService.asmx", "EAKSuperVisionService/UpdateSystemInstallStatus", string((char*) s), false);
	    cout << res;
	    if ((int) res.find(">1<", 0) != -1) {

	    }
	} else {
	    cout << "\nSNO out of range.";
	    cout << "\nSelect a system(SNO): ";
	    cin >> i;
	}
    } else {
	cout << "\nNo systems are allocated to this user. Please try again... Ctrl+c to quit.\n----------------------\n";
    }
    FILE *fp = fopen("out.xml", "w+");
    fwrite(res.c_str(), 1, res.length(), fp);
    fclose(fp);
    xmlCleanupParser();
}

int main(int argc, char** argv) {

    /*
     * std::ofstream outputFileStream;
     * outputFileStream.open("error.log", fstream::in | fstream::out | fstream::app);
     * std::streambuf * yourStreamBuffer = outputFileStream.rdbuf();
     * std::cerr.rdbuf(yourStreamBuffer);
     * 
     */

    struct sigaction sigchld_action;
    memset(&sigchld_action, 0, sizeof (sigchld_action));
    sigchld_action.sa_handler = &clean_up_child_process;
    sigaction(SIGCHLD, &sigchld_action, NULL);
    int fi = open("error.log", O_APPEND | O_WRONLY);
    dup2(fi, 2);
    int next_option;
    const char* const short_options = "hri";

    const struct option long_options[] = {
	{"help", 0, NULL, 'h'},
	{"run", 1, NULL, 'r'},
	{"install", 0, NULL, 'i'},
	{NULL, 0, NULL, 0}
    };

    do {
	next_option = getopt_long(argc, argv, short_options, long_options, NULL);
	switch (next_option) {
	    case 'h':
		print_usage(stdout, 0, argv[0]);
		break;
	    case 'r':
		run();
		break;
	    case 'i':
		install();
		break;
	    case '?':
		print_usage(stderr, 1, argv[0]);
		break;
	    case -1:
		break;
	    default:
		printf("\ninvalid option");
		break;
	}
	//abort();
	break;
    } while (next_option != -1);

    close(fi);
    /*
     * outputFileStream.close();
     */
    return 0;
}

