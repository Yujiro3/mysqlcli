/**
 * main.cpp
 *
 * C++ versions 4.4.5
 *
 *      analyzer : http://analyzer.fc2.com/
 *      Copyright (c) 2011-2013 fc2.inc All Rights Reserved.
 *
 * @package         analyzer
 * @copyright       Copyright (c) 2011-2013 fc2.inc
 * @author          Yujiro Takahashi <takahashi.yujiro@hpsys.co.jp>
 * @filesource
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <cstring>
#include <csignal>
#include <cstdlib>

#include <event2/util.h>
#include <event2/event.h>

#include "cdo_mysqlcli.h"

using namespace std;
using namespace cdo;

/**
 * メイン関数
 *
 * @access public
 * @param int  argc
 * @param char **argv
 * @return int
 */
int main(int argc, char **argv) {
    struct event_base *base;
    base = event_base_new();

    MySQLClient *mysqcli = new MySQLClient(base, "localhost", "user", "passwd", "test");
    MySQLStatement *stmt = mysqcli->prepare("SELECT * FROM `sample` WHERE `id` != ? LIMIT 5");

    int w_id = 4;
    stmt->bindParam(0, &w_id, MYSQL_TYPE_LONG);

    
    int id, value;
    char name[64];

    stmt->bind(0, &id,    sizeof(id),    MYSQL_TYPE_LONG);
    stmt->bind(1, &name,  sizeof(name),  MYSQL_TYPE_STRING);
    stmt->bind(2, &value, sizeof(value), MYSQL_TYPE_LONG);


    if (stmt->execute()) {
        while (stmt->fetch()) {
            cout << id << "\t" << name << "\t" << value << endl;
        }
    }

    cout << "/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/" << endl;

    mysqcli->query("SELECT * FROM `sample` WHERE `id` = 2");
    while (MYSQL_ROW row = mysqcli->fetch()) {
        cout << row[0] << "\t" << row[1] << "\t" << row[2] << endl;
    }
    mysqcli->freeResult();

    /* イベントループ */
    event_base_dispatch(base);


    event_base_free(base);

    delete stmt;
    delete mysqcli;

    return 0;
}