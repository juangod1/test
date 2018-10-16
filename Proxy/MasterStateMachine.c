//
// Created by juangod on 13/10/18.
//

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "include/state.h"
#include "include/stateMachine.h"
#include "include/MasterStateMachine.h"
#include "include/list.h"
#include "include/stateSelector.h"
#include <sys/select.h>

state_machine * sm;

file_descriptor MUA_sock;

state_machine * initialize_master_machine(file_descriptor MUA_sock){
    sm = new_machine();
    sm->states=new_list();

    sm->states_amount=0;
    sm->previous_state=NULL;
    sm->next_state=NULL;

    state s = new_state(ERROR_STATE, ERROR_on_arrive, ERROR_on_resume,ERROR_on_leave);
    s->socket_write_fd=-2;
    s->socket_read_fd=-2;
    put(sm->states,s);

    state connect_client = new_state(CONNECT_CLIENT_STATE, CONNECT_CLIENT_on_arrive, CONNECT_CLIENT_on_resume,CONNECT_CLIENT_on_leave);
    connect_client->socket_write_fd=-1;
    connect_client->socket_read_fd=MUA_sock;
    put(sm->states,connect_client);

    return sm;
}

execution_state ATTEND_ADMIN_on_arrive(state s, file_descriptor fd){

}

execution_state ATTEND_ADMIN_on_resume(state s, file_descriptor fd){

}

state_code ATTEND_ADMIN_on_leave(state s){

}

execution_state CONNECT_ADMIN_on_arrive(state s, file_descriptor fd){

}

execution_state CONNECT_ADMIN_on_resume(state s, file_descriptor fd){

}

state_code CONNECT_ADMIN_on_leave(state s){

}

execution_state CONNECT_CLIENT_on_arrive(state s, file_descriptor fd){
    int accept_ret = accept(MUA_sock,NULL,NULL);

    if(accept_ret<0){
        perror("accept");
        error();
    }

    state st = new_state(ATTEND_CLIENT_STATE,ATTEND_CLIENT_on_arrive,ATTEND_CLIENT_on_resume,ATTEND_CLIENT_on_leave);
    st->socket_read_fd = accept_ret;
    add_state(sm,st);



    return NOT_WAITING;
}

execution_state CONNECT_CLIENT_on_resume(state s, file_descriptor fd){

}

state_code CONNECT_CLIENT_on_leave(state s){

}

execution_state ATTEND_CLIENT_on_arrive(state s, file_descriptor fd){
    if(fd==s->pipe_write_fd) {

    }
    if(fd==s->pipe_read_fd) {

    }
    if(fd==s->socket_read_fd) {

    }
    if(fd==s->socket_write_fd) {

    }
}

execution_state ATTEND_CLIENT_on_resume(state s, file_descriptor fd){
    ATTEND_CLIENT_on_arrive(s,fd);
}

state_code ATTEND_CLIENT_on_leave(state s){

}

execution_state ERROR_on_arrive(state s, file_descriptor fd){

}

execution_state ERROR_on_resume(state s, file_descriptor fd){

}

state_code ERROR_on_leave(state s){

}

void set_up_fd_sets_rec(fd_set * read_fds, fd_set * write_fds, node curr){
    if(curr==NULL)
        return;

    if(curr->st->socket_read_fd>0)
        add_read_fd(curr->st->socket_read_fd);

    if(curr->st->socket_write_fd>0)
        add_write_fd(curr->st->socket_write_fd);

    if(curr->st->pipe_read_fd>0)
        add_read_fd(curr->st->pipe_read_fd);

    if(curr->st->pipe_write_fd>0)
        add_write_fd(curr->st->pipe_write_fd);

    set_up_fd_sets_rec(read_fds,write_fds,curr->next);
}

void set_up_fd_sets(fd_set * read_fds, fd_set * write_fds){
    set_up_fd_sets_rec(read_fds,write_fds,sm->states->head);
}