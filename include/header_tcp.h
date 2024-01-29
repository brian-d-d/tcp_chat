#ifndef __HEADER_TCP_H
#define __HEADER_TCP_H

enum header_type : char {
    username_message_ = 0,
    username_password_ = 1,
    new_username_password_ = 2,
    sign_out_ = 3,
    delete_account_ = 4,
    change_password_ = 5
};

enum header_info : char {
    header_length = sizeof(header_type)
};

#endif



