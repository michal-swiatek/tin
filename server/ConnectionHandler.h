//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_CONNECTIONHANDLER_H
#define TIN_CONNECTIONHANDLER_H

class ConnectionHandler
{
public:
    enum Requests { CONNECT, DISCONNECT, OPEN_FILE, READ_FILE, WRITE_FILE, UNLINK_FILE,
                    FILE_STAT, CLOSE_FILE, OPEN_DIR, READ_DIR, CLOSE_DIR };

    enum Replies { OK };    //  TODO: implement/find usages

    enum Errors { INVALID_COMMAND, SERVER_ERROR };  //  TODO: implement more errors

    [[nodiscard]] Requests getRequest() const;
};

#endif //TIN_CONNECTIONHANDLER_H
