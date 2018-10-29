//
// Created by juangod on 08/10/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/options.h"
#include "../Shared/include/executionValidator.h"
#include "../Shared/include/lib.h"
#include "include/proxyParse.h"

static app_context_p app_context = NULL;

static char * version_number = "PRE_ALPHA 1.0.0.0";

void initialize_app_context()
{
    app_context=malloc(sizeof(app_context_t));
    app_context->censored_media_types=NULL;
    app_context->pop3_path=0;
    app_context->command_specification=NULL;
    app_context->error_path=NULL;
    app_context->local_port=0;
    app_context->management_path=0;
    app_context->management_port=0;
    app_context->origin_port=0;
    app_context->replacement_message=NULL;
    app_context->address_server_string=NULL;
    app_context->addr=NULL;
    app_context->has_to_query_dns=0;
    app_context->pipelining=false;
    char *monitoreo[5] = {"1 - Conections", "2 - Connected Admins", "3 - Messages", "4 - Messages2", "5 - Messages3"};
    for(int i=0; i < 5; i++){
        int len = strlen(monitoreo[i]);
        (app_context->monitor)[i] = calloc(1, len+1);
        memcpy((app_context->monitor)[i], monitoreo[i], len);
        (app_context->monitor_values)[i] = -1;
    }
}

void destroy_app_context()
{
    recursiveDoublePointerFree(app_context->censored_media_types);
    free(app_context);
}

app_context_p get_app_context()
{
    return app_context;
}

void initialize_options()
{
    option_register("Error Path",'e',error_specification,"/dev/null","Specifies the file where stderr is rerouted to. By default /dev/null",error_specification_validation, 1);
    option_register("Pop3 Path",'l',pop3_direction,"all","Specifies the route where the Proxy will act.",pop3_direction_validation, 1);
    option_register("Management Path",'L',management_direction,"loopback","Specifies the route where the Management service will act.",management_direction_validation, 1);
    option_register("Replacement Message",'m',replacement_message,"Replaced Part","Specifies the message to replace filtered text.",replacement_message_validation, 1);
    option_register("Censored Mediatype",'M',censored_mediatype,"","Censored media type list",censored_mediatype_validation, 1);
    option_register("Management port",'o',management_port,"9090","Specifies the SCTP port the management server listens to. By default 9090",management_port_validation, 1);
    option_register("Local Port",'p',local_port,"1110","Specifies the TCP port for incoming POP3 connections. By default 1110",local_port_validation, 1);
    option_register("Origin Port",'P',origin_port,"110","Specifies the TCP port where the POP3 server is found in the origin server. By default 110",origin_port_validation, 1);
    option_register("Command Specification",'t',command_specification,"cat","Command utilized for external transformations. Compatible with system(3). By default applies no transformations.", command_specification_validation, 1);
}

void pop3_direction(char * arg)
{
    //TODO:
    printf("This is pop3 direction!\n");fflush(stdout);
    app_context->pop3_path=1;
}

void error_specification(char * arg)
{
    //TODO: has to receive a file direction and use it to specify the file to print out the stderr.
    printf("This is error specification!\n");fflush(stdout);
    app_context->error_path=arg;
}

void management_direction(char * arg)
{
    //TODO: has to receive a direction and use it to specify the route of the management service.
    printf("This is management direction!\n");fflush(stdout);
    app_context->management_path=1;
}
void replacement_message(char * arg)
{
    //TODO: has to receive a message and use it as a replacement for something.
    printf("This is replacement message!\n");fflush(stdout);
    app_context->replacement_message=arg;

}
void censored_mediatype(char * arg)
{
    //TODO: has to receive a list of media types and apply a condition for the server to censor them <<USE PROTOS TP 2>>
    printf("This is censored mediatype!\n");fflush(stdout);
    char ** mediaRangeCompleteList = divideMediaRangeList(arg);
    app_context->censored_media_types=mediaRangeCompleteList;

}
void management_port(char * arg)
{
    //TODO: has to receive a port and specify it as the SCTP port the management server will listen to
    printf("This is management port!\n");fflush(stdout);
    app_context->management_port=10;
}
void local_port(char * arg)
{
    //TODO: has to receive a port and specify
    printf("This is local port!\n");fflush(stdout);
    app_context->local_port=1110;
}
void origin_port(char * arg)
{
    //TODO: has to receive a port and specify it as the origin server POP3 port
    printf("This is origin port!\n");fflush(stdout);
    app_context->origin_port=110;
}
void command_specification(char * arg)
{
    //TODO: has to receive a command from stdin and apply it to the content that passes through proxy. <<USE PROTOS TP 1>>
    printf("This is command_specification!\n");fflush(stdout);
    app_context->command_specification=arg;

}
void version()
{
    //TODO: has to print out version
    printf("%s!\n",version_number);fflush(stdout);

}
void help()
{
    printf("SINOPSIS:\n\tpop3filter [ POSIX style options ] origin-server\n\tpop3filter -v\n\tpop3filter -h\n\n");
    printf("OPTIONS:\n");
    option_help();
}

void server_string(char * server_string)
{
    printf("This is server string!\n");fflush(stdout);
    int n1,n2,n3,n4;
    unsigned int m1,m2,m3,m4,m5,m6,m7,m8;
    int ret = sscanf(server_string,"%d.%d.%d.%d",&n1,&n2,&n3,&n4);
    if(ret==4){
        get_app_context()->has_to_query_dns=false;
        printf("IPv4 Address received.\n");
    }
    else{
        ret = sscanf(server_string,"%x:%x:%x:%x:%x:%x:%x:%x",&m1,&m2,&m3,&m4,&m5,&m6,&m7,&m8);
        if(ret==8){
            get_app_context()->has_to_query_dns=false;
            printf("IPv6 Address received.\n");
        }
        else{
            printf("Origin name requires DNS query.\n");
            get_app_context()->has_to_query_dns=true;
        }
    }
    app_context->address_server_string=server_string;
}