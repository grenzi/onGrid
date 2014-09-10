#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <vector>
#include <mysql.h>
#include "DBJob.h"

class DBManager {
	private:
		MYSQL *m_conn;       // holds info about the connection 
		MYSQL_STMT *m_stmt;  // pointer to handler of prepared stmt
		char *m_server;
		char *m_user;
		char *m_password;
		char *m_database;
		void init(const int &initial);
		void print_stmt_error (char *message);
	
	public:
		DBManager();  // default constructor
		DBManager(char *server, char *user, char *password, char *database, const int &initial);
		~DBManager();
		void update_processing(const int& job_id);
		void update_completed_type134(const int& job_id, const std::string& result, const int& type);
		void update_completed_type2(const int& job_id, const std::string& start_result, const std::string& end_result);
		std::vector<int> check_jobs();
		DBJob get_job(const int& id);
		void close();
};

#endif
