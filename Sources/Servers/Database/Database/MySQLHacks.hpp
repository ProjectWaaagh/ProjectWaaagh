#ifndef PROJECTWAAAGH_MYSQLHACKS_HPP
#define PROJECTWAAAGH_MYSQLHACKS_HPP

#include "Defines.hpp"
#include "MySQLWorkaround.hpp"

#include <type_traits>


struct MySQLHandle : MYSQL {};
struct MySQLResult : MYSQL_RES {};
struct MySQLField : MYSQL_FIELD {};
struct MySQLBind : MYSQL_BIND {};
struct MySQLStmt : MYSQL_STMT {};

using MySQLBool = std::remove_pointer_t<decltype(std::declval<MYSQL_BIND>().is_null)>;

#endif //PROJECTWAAAGH_MYSQLHACKS_HPP
