#ifndef __HEADER_TCP_H
#define __HEADER_TCP_H


enum header_info : char {
    header_length = 1
};

enum header_type : char {
    username_message = 0,
    username_password = 1,
    create_account = 2
};

#endif



