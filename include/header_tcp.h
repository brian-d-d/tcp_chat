#ifndef __HEADER_TCP_H
#define __HEADER_TCP_H

enum header_type : char {
    username_message = 0,
    username_password = 1,
    new_username_password = 2,
    sign_out = 3,
    remove_account = 4
};

enum header_info : char {
    header_length = sizeof(header_type)
};

#endif



