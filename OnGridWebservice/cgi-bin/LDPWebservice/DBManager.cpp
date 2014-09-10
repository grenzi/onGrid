#include "DBManager.h"
#include "Util.h"
#include <iostream>

DBManager::DBManager() {
	// nothing
}

DBManager::DBManager(char *server, char *user, char *password, char *database, const int &initial) : m_server(server), m_user(user), m_password(password), m_database(database) {
	init(initial);
 }
 
 DBManager::~DBManager() {
 }
 
 void DBManager::print_stmt_error (char *message) {
	fprintf (stderr, "%s\n", message);
	if (m_stmt != NULL) {
		fprintf (stderr, "Error %u (%s): %s\n",
		mysql_stmt_errno (m_stmt),
		mysql_stmt_sqlstate (m_stmt),
		mysql_stmt_error (m_stmt));
	}
}
 
 void DBManager::close() {
	// deallocate statement handler
	mysql_stmt_close(m_stmt);
	// close connection
	mysql_close(m_conn);
 }
 
 void DBManager::init(const int &initial) {
	if (initial == 1) {
			// call to initialize MySQL library
		if (mysql_library_init(0, NULL, NULL)) {
			fprintf(stderr, "could not initialize MySQL library\n");
			exit(1);
		}
	}

	// Connect to database
	// NOTE: mysql_init() has the side effect of calling mysql_thread_init()
	m_conn = mysql_init(NULL);
	if (!mysql_real_connect(m_conn, m_server, m_user, m_password, m_database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(m_conn));
		exit(1);
	}

	m_stmt = mysql_stmt_init(m_conn);  // allocate statement handler
	if (m_stmt == NULL) {
		print_stmt_error("could not prepare statement");
		exit(1);
	}
 }

void DBManager::update_processing(const int& job_id) {
	char *stmt_str;
	stmt_str = "UPDATE wkdd_ldp_jobs SET status='processing' WHERE id = ?";
	
	MYSQL_BIND param[1];
	int my_job_id;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_job_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	// buffer length, length need not be set
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	my_job_id = job_id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute statement");
		return;
	}
}

void DBManager::update_completed_type134(const int& job_id, const std::string& result, const int& type) {
	char *stmt_str;
	if (type == 1)
		stmt_str = "UPDATE wkdd_ldp_jobs SET top_cui = ?, status = 'complete', end_time = ? WHERE id = ?";
	else if (type == 3)
		stmt_str = "UPDATE wkdd_ldp_jobs SET start_cui_option = ?, status = 'complete', end_time = ? WHERE id = ?";
	else if (type == 4)
		stmt_str = "UPDATE wkdd_ldp_jobs SET end_cui_option = ?, status = 'complete', end_time = ? WHERE id = ?";
		
	MYSQL_BIND param[3];
	int my_job_id;
	char my_cui[result.length()];
	unsigned long my_cui_length;
	MYSQL_TIME my_datetime;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_cui parameter
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = (void *) &my_cui;
	param[0].buffer_length = sizeof(my_cui);
	param[0].length = &my_cui_length;
	param[0].is_null = 0;
	
	// set up my_datetime
	param[1].buffer_type = MYSQL_TYPE_DATETIME;
	param[1].buffer = (void *) &my_datetime;
	param[1].is_null = 0;
	
	// set up my_id parameter
	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = (void *) &my_job_id;
	param[2].is_unsigned = 0;
	param[2].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	// get current time
	(void)time (&clock);
	
	// set variables
	my_job_id = job_id;
	(void) strcpy(my_cui, result.c_str());
	my_cui_length = strlen(my_cui);
	cur_time = localtime(&clock);
	my_datetime.year = cur_time->tm_year + 1900;
	my_datetime.month = cur_time->tm_mon + 1;
	my_datetime.day = cur_time->tm_mday;
	my_datetime.hour = cur_time->tm_hour;
	my_datetime.minute = cur_time->tm_min;
	my_datetime.second = cur_time->tm_sec;
	my_datetime.second_part = 0;
	my_datetime.neg = 0;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute statement");
		return;
	}
}

void DBManager::update_completed_type2(const int& job_id, const std::string& start_result, const std::string& end_result) {
	char *stmt_str = "UPDATE wkdd_ldp_jobs SET start_cui_option = ?, end_cui_option = ?, status = 'complete', end_time = ? WHERE id = ?";
	MYSQL_BIND param[4];
	int my_job_id;
	char my_startcui[start_result.length()];
	char my_endcui[end_result.length()];
	unsigned long my_startcui_length;
	unsigned long my_endcui_length;
	MYSQL_TIME my_datetime;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_startcui parameter
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = (void *) &my_startcui;
	param[0].buffer_length = sizeof(my_startcui);
	param[0].length = &my_startcui_length;
	param[0].is_null = 0;
	
	// set up my_endcui parameter
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = (void *) &my_endcui;
	param[1].buffer_length = sizeof(my_endcui);
	param[1].length = &my_endcui_length;
	param[1].is_null = 0;
	
	// set up my_datetime
	param[2].buffer_type = MYSQL_TYPE_DATETIME;
	param[2].buffer = (void *) &my_datetime;
	param[2].is_null = 0;
	
	// set up my_id parameter
	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = (void *) &my_job_id;
	param[3].is_unsigned = 0;
	param[3].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	// get current time
	(void)time (&clock);
	
	// set variables
	my_job_id = job_id;
	(void) strcpy(my_startcui, start_result.c_str());
	(void) strcpy(my_endcui, end_result.c_str());
	my_startcui_length = strlen(my_startcui);
	my_endcui_length = strlen(my_endcui);
	cur_time = localtime(&clock);
	my_datetime.year = cur_time->tm_year + 1900;
	my_datetime.month = cur_time->tm_mon + 1;
	my_datetime.day = cur_time->tm_mday;
	my_datetime.hour = cur_time->tm_hour;
	my_datetime.minute = cur_time->tm_min;
	my_datetime.second = cur_time->tm_sec;
	my_datetime.second_part = 0;
	my_datetime.neg = 0;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute statement");
		return;
	}
}

std::vector<int> DBManager::check_jobs() {
	std::vector<int> job_ids;

	char *stmt_str = "SELECT id FROM wkdd_ldp_jobs WHERE status = 'pending' ORDER BY start_time ASC";
	MYSQL_BIND result[1];
	int my_job_id;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return job_ids;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 1) {
		print_stmt_error("unexpected column count from select");
		return job_ids;
	}
	
	// initialize result column structures
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = (void *) &my_job_id;
	result[0].is_unsigned = 0;
	result[0].is_null = 0;
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could not bind parameters for select");
		return job_ids;
	}
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return job_ids;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return job_ids;
	}
	
	while (mysql_stmt_fetch(m_stmt) == 0) {  // fetch each row
		// store into vector
		job_ids.push_back(my_job_id);
	}
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return job_ids;
}

DBJob DBManager::get_job(const int& id) {
	DBJob data;

	char *stmt_str = "SELECT term, type FROM wkdd_ldp_jobs WHERE id = ?";
	MYSQL_BIND param[1];
	MYSQL_BIND result[2];
	int my_job_id;
	int my_type;
	char my_term[2000];
	unsigned long my_term_length;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return data;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 2) {
		print_stmt_error("unexpected column count from select");
		return data;
	}
	
	// initialize result column structures
	memset((void *)param, 0, sizeof(param));
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_job_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_term parameter
	result[0].buffer_type = MYSQL_TYPE_STRING;
	result[0].buffer = (void *) &my_term;
	result[0].buffer_length = sizeof(my_term);
	result[0].length = &my_term_length;
	result[0].is_null = 0;
	
	// set up my_type parameter
	result[1].buffer_type = MYSQL_TYPE_LONG;
	result[1].buffer = (void *) &my_type;
	result[1].is_unsigned = 0;
	result[1].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for select");
		return data;
	}
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could not bind parameters for select");
		return data;
	}
	
	// set variable
	my_job_id = id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return data;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return data;
	}
	
	mysql_stmt_fetch(m_stmt);  // fetch the single result
	data = DBJob(my_term, my_type);
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return data;
}

