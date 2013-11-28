/**
 * cdo_mysqlstmt.cpp
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

#include <mysql.h>

#include "cdo_mysqlstmt.h"

namespace cdo {
    /**
     * コンストラクタ
     *
     * @access public
     * @access public
     * @param MYSQL *mysql
     * @param const char *query
     */
    MySQLStatement::MySQLStatement(MYSQL *mysql, const char *query) {
        is_error = false;
        stmt = mysql_stmt_init(mysql);
        int length = strlen(query);
        if (mysql_stmt_prepare(stmt, query, length)) {
            errmsg = mysql_error(mysql);
            is_error = true;
            return;
        }

        params = NULL;
        int count = mysql_stmt_param_count(stmt);
        if (count) {
            params = new MYSQL_BIND[count];
        }
    }

    /**
     * デストラクタ
     *
     * @access public
     */
    MySQLStatement::~MySQLStatement() {
        mysql_stmt_free_result(stmt);
        delete[] params;
    }

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
    bool MySQLStatement::bind(int pos, void *value, size_t length, enum enum_field_types type) {
        if (is_error) {
            return false;
        }

        MYSQL_BIND bind;
        bzero(&bind, sizeof(MYSQL_BIND));

        bind.buffer_type   = type;
        bind.buffer        = (char *)value;
        bind.buffer_length = length;
        bind.is_null       = 0;
        
        bvalue.push_back(bind);

        return true;
    }

    /**
     * プレースホルダに値をバインド
     *
     * @access public
     * @param int pos
     * @param char *value
     * @param enum enum_field_types type
     * @param int is_null
     * @return bool
     */
    bool MySQLStatement::bindParam(int pos, void *value, enum enum_field_types type, my_bool is_unsigned) {
        if (is_error) {
            return false;
        }

        bzero(&params[pos], sizeof(MYSQL_BIND));
        if (MYSQL_TYPE_NULL == type) {
            params[pos].buffer_type   = type;
            params[pos].is_null_value = 1;
        } else {
            params[pos].buffer_type   = type;
            params[pos].buffer        = (char *)value;
            params[pos].is_unsigned   = is_unsigned;
        }
        
        return true;
    }

    /**
     * DELETE, INSERT, UPDATE 文によって作用した行数
     *
     * @access public
     * @return my_ulonglong
     */
    my_ulonglong MySQLStatement::rowCount() {
        return mysql_affected_rows(mysql);
    }
    
    /**
     * プリペアドステートメントを実行
     *
     * @access public
     * @return bool
     */
    bool MySQLStatement::execute() {
        if (is_error) {
            return false;
        }
        if (params != NULL) {
            if (mysql_stmt_bind_param(stmt, params)) {
                errmsg = mysql_stmt_error(stmt);
                is_error = true;
                return false;
            }
        } // if (params != NULL)

        if (mysql_stmt_bind_result(stmt, &bvalue[0])) {
            errmsg = mysql_stmt_error(stmt);
            is_error = true;
            return false;
        }

        if (mysql_stmt_execute(stmt)) {
            errmsg = mysql_stmt_error(stmt);
            is_error = true;
            return false;
        }
        return true;
    }
    
    /**
     * 結果セットの1行取得
     *
     * @access public
     * @return string
     */
    bool MySQLStatement::fetch() {
        if (is_error) {
            return false;
        }

        int result = mysql_stmt_fetch(stmt);

        if (MYSQL_NO_DATA == result) {
            errmsg = mysql_stmt_error(stmt);
            is_error = true;
            return false;
        }
        return true;
    }
} // namespace cdo