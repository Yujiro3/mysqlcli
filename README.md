C++のMySQLクライアントサンプル
======================
libevent2を利用したMySQLクライアント

利用方法
------

### libevent2モジュールのインストール ###
    
    $ wget http://jaist.dl.sourceforge.net/project/levent/libevent/libevent-2.0/libevent-2.0.21-stable.tar.gz
    $ tar xzvf libevent-2.0.21-stable.tar.gz
    $ cd libevent-2.0.21-stable
    $ ./configure
    $ make
    $ sudo make install

### コンパイル ###
    
    $ g++ -L/usr/local/lib  -I/usr/local/include -o rediscli main.cpp cdo_mysqlcli.cpp cdo_mysqlstmt.cpp -lmysqlclient -levent
        

### コマンド ###

```sql
CREATE TABLE IF NOT EXISTS `sample` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(64) NOT NULL,
  `value` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `sample` (`id`, `name`, `value`) VALUES ('1', 'name1', '1'), ('2', 'name2', '2'), ('3', 'name3', '3');
```

### コマンド ###

```cpp
/* サーバ接続 */
MySQLClient *mysqcli = new MySQLClient(base, "localhost", "user", "passwd", "test");

/* ステートメント発行 */
MySQLStatement *stmt = mysqcli->prepare("SELECT * FROM `sample` WHERE `id` != ? LIMIT 5");

/* WHERE値の入力 */
int w_id = 4;
stmt->bindParam(0, &w_id, MYSQL_TYPE_LONG);

/* データの取得 */
int id, value;
char name[64];

stmt->bind(0, &id,    sizeof(id),    MYSQL_TYPE_LONG);
stmt->bind(1, &name,  sizeof(name),  MYSQL_TYPE_STRING);
stmt->bind(2, &value, sizeof(value), MYSQL_TYPE_LONG);

/* 結果の取得 */
if (stmt->execute()) {
    while (stmt->fetch()) {
        cout << id << "\t" << name << "\t" << value << endl;
    }
}
```

### テスト実行 ###
    
    $ ./mysqlcli
    1   name1   1
    2   name2   2
    3   name3   3

ライセンス
----------
Copyright &copy; 2013 Yujiro Takahashi  
Licensed under the [MIT License][MIT].  
Distributed under the [MIT License][MIT].  

[MIT]: http://www.opensource.org/licenses/mit-license.php
