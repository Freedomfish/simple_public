CREATE TABLE `mysql_route` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `start_index` int(11) DEFAULT '0' COMMENT '起始index',
  `end_index` int(11) DEFAULT '0' COMMENT '结束index',
  `rw_flag` int(11) NOT NULL DEFAULT '1' COMMENT '读写标记，1表示读，2表示写',
  `host` varchar(50) CHARACTER SET latin1 NOT NULL DEFAULT '' COMMENT 'ip',
  `port` int(11) NOT NULL DEFAULT '3306' COMMENT '端口',
  `username` varchar(50) CHARACTER SET latin1 DEFAULT '' COMMENT '用户名',
  `password` varchar(16) CHARACTER SET latin1 DEFAULT '' COMMENT '密码',
  `db` varchar(50) CHARACTER SET latin1 DEFAULT '' COMMENT '数据库名',
  `db_type` varchar(50) CHARACTER SET latin1 DEFAULT '' COMMENT '数据类型(如offlinemsg,userinfo)',
  `reserve1` varchar(50) CHARACTER SET latin1 DEFAULT '' COMMENT '预留字段',
  `reserve2` varchar(50) CHARACTER SET latin1 DEFAULT '' COMMENT '预留字段',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
