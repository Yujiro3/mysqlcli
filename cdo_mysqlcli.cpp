/**
 * cdb_mysqlcli.cpp
 *
 * C++ versions 4.4.5
 *
 *      MySQLClient : https://github.com/Yujiro3/rediscli
 *      Copyright (c) 2011-2013 sheeps.me All Rights Reserved.
 *
 * @package         MySQLClient
 * @copyright       Copyright (c) 2011-2013 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

#include <iostream>
#include <string>

#include <cstring>
#include <cstdlib>

#include "cdo_mysqlcli.h"
#include "cdo_mysqlstmt.h"

namespace cdo {
    /**
     * コンストラクタ
     *
     * @access public
     * @param struct event_base *base
     * @param string _listen
     * @param int _port
     */
    MySQLClient::MySQLClient(struct event_base *base, const char *server, const char *user, const char *password, const char *database, const short port) {
        mysql   = NULL;
        options = EV_PERSIST | EV_ET;

        this->base     = base;
        this->server   = server;
        this->user     = user;
        this->password = password;
        this->database = database;
        this->port     = port;
        
        connection();

        evmysql = event_new(base, mysql->net.fd, options, eventcb, this);
        if (!evmysql || event_add(evmysql, NULL) < 0) {
            throw "Could not create/add a mysql event!";
        }

        timeout = event_new(base, -1, EV_PERSIST, timeoutcb, this);
        if (!timeout) {
            throw "Could not create/add a timeout event!";
        }
    }

    /**
     * デストラクタ
     *
     * @access public
     */
    MySQLClient::~MySQLClient() {
        event_free(timeout);
        event_free(evmysql);
    }

    /**
     * サーバへ接続
     *
     * @access public
     * @return bool
     */
    bool MySQLClient::connection() {
        mysql = mysql_init(mysql);
        if (!mysql_real_connect(mysql, server, user, password, database, port, NULL, 0)) {
            throw mysql_error(mysql);
        }
#if 0
        /* mysql_pingだとlibeventで管理できない？ */
        my_bool reconnect = 1;
        mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
#endif
        return true;
    }

    /**
     * イベントコールバック
     *
     * @access public
     * @param evutil_socket_t fd
     * @param short events
     * @param void *argv
     * @return void
     */
    void MySQLClient::eventcb(evutil_socket_t fd, short events, void *argv) {
        if (events & (EV_PERSIST | EV_ET)) {
            MySQLClient *self = (MySQLClient *) argv;
            if (mysql_ping(self->mysql) == 0) {
                return ;
            }

            struct timeval tv;
            evutil_timerclear(&tv);
            tv.tv_sec = RETRY_TIME;

            event_add(self->timeout, &tv);
            event_del(self->evmysql);
        }
    }
    
    /**
     * タイムアウトコールバック
     *
     * @access public
     * @param evutil_socket_t fd
     * @param short events
     * @param void *argv
     * @return void
     */
    void MySQLClient::timeoutcb(evutil_socket_t fd, short events, void *argv) {
        MySQLClient *self = (MySQLClient *) argv;

        self->connection();
        event_assign(self->evmysql, self->base, self->mysql->net.fd, self->options, eventcb, argv);
        if (!self->evmysql || event_add(self->evmysql, NULL) < 0) {
            throw "Could not create/add a mysqlevent event!";
        }
        event_del(self->timeout);
    }

    /**
     * 最後に挿入された行IDの取得
     *
     * @access public
     * @param string cmd
     * @return my_ulonglong
     */
    my_ulonglong MySQLClient::lastInsertId() {
        return mysql_insert_id(mysql);
    }

    /**
     * SQL ステートメントを準備
     *
     * @access public
     * @param  const char *query
     * @return MySQLStatement
     */
    MySQLStatement *MySQLClient::prepare(const char *query) {
        return new MySQLStatement(mysql, query);
    }

    /**
     * SQL ステートメントを準備
     *
     * @access public
     * @param  string $statement
     * @return void
     */
    bool MySQLClient::query(std::string statement) {
        if (mysql_real_query(mysql, statement.c_str(), statement.length())) {
            errmsg.assign(mysql_error(mysql));
            return false;
        }
        res = mysql_use_result(mysql);

        return true;
    }

    /**
     * 結果セットの1行取得
     *
     * @access public
     * @return string
     */
    char **MySQLClient::fetch() {
        return mysql_fetch_row(res);
    }

    /**
     * 結果セットの開放
     *
     * @access public
     * @return void
     */
    void MySQLClient::freeResult() {
        mysql_free_result(res);
    }

    /**
     * SQL ステートメントを準備
     *
     * @access public
     * @param  string $statement
     * @return void
     */
    std::string MySQLClient::quote(const char *from) {
        char to[256];
        mysql_real_escape_string(mysql, to, from, strlen(from));
        
        return std::string(to);
    }

    /**
     * トランザクション開始
     *
     * @access public
     * @return bool
     */
    bool MySQLClient::beginTransaction() {
        if (mysql_autocommit(mysql, 0)) {
            return false;
        }
        return true;
    }

    /**
     * コミット
     *
     * @access public
     * @return bool
     */
    bool MySQLClient::commit() {
        bool result = mysql_commit(mysql);
        mysql_autocommit(mysql, 1);
        return result;
    }

    /**
     * トロールバック
     *
     * @access public
     * @return bool
     */
    bool MySQLClient::rollBack () {
        bool result = mysql_rollback(mysql);
        mysql_autocommit(mysql, 1);
        return result;
    }

} // namespace mem