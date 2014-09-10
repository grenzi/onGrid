#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <vector>
#include <mysql.h>
#include "Singledata.h"
#include "Matrixdata.h"

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
		void update_processing(const int &job_id, const int &type);
		void update_single_completed(const int &job_id, const int &distance, const std::string &paths);
		void update_matrix_completed(const int &job_id);
		void update_matrix_error(const int &job_id);
		void update_rejected(const int &job_id, const int &type);
		void set_slot(const int &slot_id, const long &thread_id, const int &job_id, const int &type);   // 0 for single job, 1 for matrix
		std::vector<int> get_free_slot_ids(const int &limit);
		void release_slot(const int &slot_id);
		std::vector<int> check_single_jobs();
		std::vector<int> check_matrix_jobs();
		Singledata get_single_job(const int& id);
		Matrixdata get_matrix_job(const int& id);
		bool user_email_exists(const int& id);
		void close();
};

#endif
