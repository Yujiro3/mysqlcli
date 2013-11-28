/**
 * cdo_mysqlcli.h
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

#ifndef __CDB_MYSQLCLI_H__
#define __CDB_MYSQLCLI_H__

#include <string>

#include <event2/util.h>
#include <event2/event.h>
#include <mysql.h>

#include "cdo_mysqlstmt.h"

namespace cdo {
    /**
     * リトライ秒数
     * @const unsigned int
     */
    const unsigned int RETRY_TIME = 2;

    /**
     * MySQLClientクラス
     *
     * @package analyzer
     * @author  Yujiro Takahashi <takahashi.yujiro@hpsys.co.jp>
     */
    class MySQLClient {
    public:
        /**
         * MySQL 接続
         * @var MYSQL
         */
        MYSQL *mysql;

        /**
         * イベントベース
         * @var event_base
         */
        struct event_base *base;

        /**
         * MySQL 接続
         * @var MYSQL
         */
        struct event *timeout;

        /**
         * MySQL 接続
         * @var MYSQL
         */
        struct event *evmysql;

        /**
         * イベントオプション
         * @var short
         */
        short options;

        /**
         * DBサーバ名
         * @var const char *
         */
        const char *server;

        /**
         * DB接続ユーザー
         * @var const char *
         */
        const char *user;

        /**
         * DBパスワード
         * @var const char *
         */
        const char *password;

        /**
         * DB名
         * @var const char *
         */
        const char *database;

        /**
         * DBポート
         * @var short
         */
        short port;

        /**
         * 結果
         * @var MYSQL_RES
         */
        MYSQL_RES *res;

        /**
         * エラーメッセージ
         * @var string
         */
        std::string errmsg;

    public:
        /**
         * コンストラクタ
         *
         * @access public
         * @param struct event_base *base
         * @param string _listen
         * @param int _port
         */
        MySQLClient(struct event_base *base, const char *server, const char *user, const char *password, const char *database, const short port=0);
    
        /**
         * デストラクタ
         *
         * @access public
         */
        ~MySQLClient();
    
        /**
         * サーバへ接続
         *
         * @access public
         * @return bool
         */
        bool connection();
    
        /**
         * イベントコールバック
         *
         * @access public
         * @param evutil_socket_t fd
         * @param short events
         * @param void *argv
         * @return void
         */
        static void eventcb(evutil_socket_t fd, short events, void *argv);
        
        /**
         * タイムアウトコールバック
         *
         * @access public
         * @param evutil_socket_t fd
         * @param short events
         * @param void *argv
         * @return void
         */
        static void timeoutcb(evutil_socket_t fd, short events, void *argv);
    
        /**
         * 最後に挿入された行IDの取得
         *
         * @access public
         * @param string cmd
         * @return my_ulonglong
         */
        my_ulonglong lastInsertId();
    
        /**
         * SQL ステートメントを準備
         *
         * @access public
         * @param  const char *query
         * @return MySQLStatement
         */
        MySQLStatement *prepare(const char *query);
    
        /**
         * SQL ステートメントを準備
         *
         * @access public
         * @param  string $statement
         * @return void
         */
        bool query(std::string statement);

        /**
         * 結果セットの1行取得
         *
         * @access public
         * @return string
         */
        char **fetch();

        /**
         * 結果セットの開放
         *
         * @access public
         * @return void
         */
        void freeResult();

        /**
         * SQL ステートメントを準備
         *
         * @access public
         * @param  string $statement
         * @return void
         */
        std::string quote(const char *from);
    
        /**
         * トランザクション開始
         *
         * @access public
         * @return bool
         */
        bool beginTransaction();
    
        /**
         * コミット
         *
         * @access public
         * @return bool
         */
        bool commit();
    
        /**
         * トロールバック
         *
         * @access public
         * @return bool
         */
        bool rollBack();
    }; // class MySQLClient
} // namespace mem

#endif // #ifndef __CDB_MYSQLCLI_H__