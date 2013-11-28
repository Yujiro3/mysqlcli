/**
 * cdo_mysqlstmt.h
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

#ifndef __CDO_MYSQLSTMT_H__
#define __CDO_MYSQLSTMT_H__

#include <string>
#include <map>
#include <vector>

#include <mysql.h>

namespace cdo {
    /**
     * 2次元配列コンテナの型定義
     * @typedef typedef std::map<std::string, std::string>
     */
    typedef std::map<std::string, std::string> array_t;

    /**
     * MySQLStatementクラス
     *
     * @package analyzer
     * @author  Yujiro Takahashi <takahashi.yujiro@hpsys.co.jp>
     */
    class MySQLStatement {
    public:
        /**
         * MySQL 接続
         * @var MYSQL
         */
        MYSQL *mysql;

        /**
         * ステートメント
         * @var MYSQL_STMT
         */
        MYSQL_STMT *stmt;

        /**
         * バインド
         * @var MYSQL_BIND
         */
        MYSQL_BIND *params;

        /**
         * バインド
         * @var MYSQL_BIND
         */
        std::vector<MYSQL_BIND> bvalue;

        /**
         * エラーメッセージ
         * @var string
         */
        std::string errmsg;

        /**
         * エラー
         * @var boolean
         */
        bool is_error;

    public:
        /**
         * コンストラクタ
         *
         * @access public
         * @param MYSQL *mysql
         * @param const char *query
         */
        MySQLStatement(MYSQL *mysql, const char *query);

        /**
         * デストラクタ
         *
         * @access public
         */
        ~MySQLStatement();

        /**
         * プレースホルダに値をバインド
         *
         * @access public
         * @param int pos
         * @param void *value
         * @param size_t length
         * @param enum enum_field_types type
         * @return bool
         */
        bool bind(int pos, void *value, size_t length, enum enum_field_types type);

        /**
         * プレースホルダに値をバインド
         *
         * @access public
         * @param int pos
         * @param void *value
         * @param enum enum_field_types type
         * @param my_bool is_unsigned
         * @return bool
         */
        bool bindParam(int pos, void *value, enum enum_field_types type, my_bool is_unsigned=1);
        
        /**
         * DELETE, INSERT, UPDATE 文によって作用した行数
         *
         * @access public
         * @return my_ulonglong
         */
        my_ulonglong rowCount();
        
        /**
         * プリペアドステートメントを実行
         *
         * @access public
         * @return bool
         */
        bool execute();
        
        /**
         * 結果セットの1行取得
         *
         * @access public
         * @return bool
         */
        bool fetch();
    
        /**
         * 結果セットの全行取得
         *
         * @access public
         * @return array_t
         */
        void fetchAll();

        /**
         * 結果セットの1カラム取得
         *
         * @access public
         * @return string
         */
        std::string fetchColumn();
    }; // class MySQLStatement
} // namespace cdo

#endif // #ifndef __CDO_MYSQLSTMT_H__