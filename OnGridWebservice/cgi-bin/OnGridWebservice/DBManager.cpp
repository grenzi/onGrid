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

void DBManager::update_processing(const int &job_id, const int &type) {
	char *stmt_str;
	if (type == 0) {// single
		stmt_str = "UPDATE wkdd_single_jobs SET status='processing' WHERE id = ?";
	}
	else {  // matrix
		stmt_str = "UPDATE wkdd_matrix_jobs SET status='processing' WHERE id = ?";
	}
	
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

void DBManager::update_single_completed(const int &job_id, const int &distance, const std::string &paths) {
	char *stmt_str = "UPDATE wkdd_single_jobs SET distance = ?, paths = ?, status='complete', end_time = ? WHERE id = ?";
	MYSQL_BIND param[4];
	int my_job_id, my_distance;
	char my_paths[paths.length()];
	unsigned long my_paths_length;
	MYSQL_TIME my_datetime;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_distance parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_distance;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_paths parameter
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = (void *) &my_paths;
	param[1].buffer_length = sizeof(my_paths);
	param[1].length = &my_paths_length;
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
	my_distance= distance;
	my_job_id = job_id;
	(void) strcpy(my_paths, paths.c_str());
	my_paths_length = strlen(my_paths);
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

void DBManager::update_matrix_completed(const int &job_id) {
	char *stmt_str = "UPDATE wkdd_matrix_jobs SET status='complete', end_time = ? WHERE id = ?";
	MYSQL_BIND param[2];
	int my_job_id;
	MYSQL_TIME my_datetime;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_datetime
	param[0].buffer_type = MYSQL_TYPE_DATETIME;
	param[0].buffer = (void *) &my_datetime;
	param[0].is_null = 0;
	
	// set up my_id parameter
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = (void *) &my_job_id;
	param[1].is_unsigned = 0;
	param[1].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	// get current time
	(void)time (&clock);
	
	// set variables
	my_job_id = job_id;
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

void DBManager::update_matrix_error(const int &job_id) {
	char *stmt_str = "UPDATE wkdd_matrix_jobs SET status='error', end_time = ? WHERE id = ?";
	MYSQL_BIND param[2];
	int my_job_id;
	MYSQL_TIME my_datetime;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_datetime
	param[0].buffer_type = MYSQL_TYPE_DATETIME;
	param[0].buffer = (void *) &my_datetime;
	param[0].is_null = 0;
	
	// set up my_id parameter
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = (void *) &my_job_id;
	param[1].is_unsigned = 0;
	param[1].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	// get current time
	(void)time (&clock);
	
	// set variables
	my_job_id = job_id;
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

void DBManager::update_rejected(const int &job_id, const int &type) {
	char *stmt_str;
	if (type == 0) {
		stmt_str = "UPDATE wkdd_single_jobs SET status='rejected' WHERE id = ?";
	}
	else {
		stmt_str = "UPDATE wkdd_matrix_jobs SET status='rejected' WHERE id = ?";
	}
	
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
	
	// set variables
	my_job_id = job_id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute statement");
		return;
	}
}

void DBManager::set_slot(const int &slot_id, const long &thread_id, const int &job_id, const int &type) {
	char *stmt_str = "UPDATE wkdd_slots SET thread_id = ?, job_id = ?, state = '1', start_time = ?, end_time = NULL WHERE id = ?";
	MYSQL_BIND param[4];
	int my_slot_id;
	
	std::string s_job_id;
	if (type == 0) {
		s_job_id = "s_";
	}
	else {
		s_job_id = "m_";
	}
	s_job_id += Util::numToStr(job_id);
	char my_job_id[s_job_id.length()];
	
	unsigned long my_job_id_length;
	long my_thread_id;
	MYSQL_TIME my_datetime;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	memset((void *)param, 0, sizeof(param));
	
	// set up my_thread_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONGLONG;  // use LONGLONG to store BIGINT, larger than 10 values
	param[0].buffer = (void *) &my_thread_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_job_id, as string
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = (void *) &my_job_id;
	param[1].buffer_length = sizeof(my_job_id);
	param[1].length = &my_job_id_length;
	param[1].is_null = 0;
	
	// set up my_datetime
	param[2].buffer_type = MYSQL_TYPE_DATETIME;
	param[2].buffer = (void *) &my_datetime;
	param[2].is_null = 0;
	
	// set up my_slot_id parameter
	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = (void *) &my_slot_id;
	param[3].is_unsigned = 0;
	param[3].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	// get current time
	(void)time (&clock);
	
	// set variables
	my_slot_id = slot_id;
	my_thread_id = thread_id;
	(void) strcpy(my_job_id, s_job_id.c_str());
	my_job_id_length = strlen(my_job_id);
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

std::vector<int> DBManager::get_free_slot_ids(const int &limit) {

	char *stmt_str = "SELECT id FROM wkdd_slots WHERE state = '0' ORDER BY id ASC LIMIT ?";
	MYSQL_BIND param[1];
	MYSQL_BIND result[1];
	int my_slot_id;
	int my_limit;
	std::vector<int> slot_ids;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
	}
	
	if (mysql_stmt_field_count(m_stmt) != 1) {
		print_stmt_error("unexpected column count from select");
	}
	
	// initialize result column structures
	memset((void *)param, 0, sizeof(param)); // zero the structures
	memset((void *)result, 0, sizeof(result)); // zero the structures
		
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_slot_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_limit parameter
	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = (void *) &my_limit;
	result[0].is_unsigned = 0;
	result[0].is_null = 0;
	
	if (mysql_stmt_bind_result(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for select");
		return slot_ids;
	}
	
	if (mysql_stmt_bind_param(m_stmt, result) != 0) {
		print_stmt_error("could not bind parameters for select...");
		return slot_ids;
	}
	
	// set value
	my_limit = limit;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return slot_ids;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error( "could not buffer result set");
		return slot_ids;
	}
	
	while (mysql_stmt_fetch(m_stmt) == 0) {
		slot_ids.push_back(my_slot_id);
	}
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return slot_ids;
}

void DBManager::release_slot(const int &slot_id) {
	char *stmt_str = "UPDATE wkdd_slots SET state = '0', end_time = ? WHERE id = ?";
	MYSQL_BIND param[2];
	int my_slot_id;
	MYSQL_TIME my_datetime;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	memset((void *)param, 0, sizeof(param));
	
	// set up my_datetime
	param[0].buffer_type = MYSQL_TYPE_DATETIME;
	param[0].buffer = (void *) &my_datetime;
	param[0].is_null = 0;
	
	// set up my_slot_id parameter
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = (void *) &my_slot_id;
	param[1].is_unsigned = 0;
	param[1].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	// get current time
	(void)time (&clock);
	
	// set variables
	my_slot_id = slot_id;
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

std::vector<int> DBManager::check_single_jobs() {
	std::vector<int> single_job_ids;

	char *stmt_str = "SELECT id FROM wkdd_single_jobs WHERE status = 'pending' ORDER BY start_time ASC";
	MYSQL_BIND result[1];
	int my_job_id;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return single_job_ids;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 1) {
		print_stmt_error("unexpected column count from select");
		return single_job_ids;
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
		return single_job_ids;
	}
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return single_job_ids;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return single_job_ids;
	}
	
	while (mysql_stmt_fetch(m_stmt) == 0) {  // fetch each row
		// store into vector
		single_job_ids.push_back(my_job_id);
	}
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return single_job_ids;
}

Singledata DBManager::get_single_job(const int& id) {
	Singledata single_data;

	char *stmt_str = "SELECT start_id, end_id FROM wkdd_single_jobs WHERE id = ?";
	MYSQL_BIND param[1];
	MYSQL_BIND result[2];
	int my_job_id;
	char my_start_id[45];
	char my_end_id[45];
	unsigned long my_start_id_length, my_end_id_length;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return single_data;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 2) {
		print_stmt_error("unexpected column count from select");
		return single_data;
	}
	
	// initialize result column structures
	memset((void *)param, 0, sizeof(param));
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_job_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_startid parameter
	result[0].buffer_type = MYSQL_TYPE_STRING;
	result[0].buffer = (void *) &my_start_id;
	result[0].buffer_length = sizeof(my_start_id);
	result[0].length = &my_start_id_length;
	result[0].is_null = 0;
	
	// set up my_endid parameter
	result[1].buffer_type = MYSQL_TYPE_STRING;
	result[1].buffer = (void *) &my_end_id;
	result[1].buffer_length = sizeof(my_end_id);
	result[1].length = &my_end_id_length;
	result[1].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for select");
		return single_data;
	}
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could not bind parameters for select");
		return single_data;
	}
	
	// set variable
	my_job_id = id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return single_data;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return single_data;
	}
	
	mysql_stmt_fetch(m_stmt);  // fetch the single result
	single_data = Singledata(id, my_start_id, my_end_id);
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return single_data;
}

std::vector<int> DBManager::check_matrix_jobs() {
	std::vector<int> matrix_job_ids;

	char *stmt_str = "SELECT id FROM wkdd_matrix_jobs WHERE status = 'pending' ORDER BY start_time ASC";
	MYSQL_BIND result[1];
	int my_job_id;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return matrix_job_ids;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 1) {
		print_stmt_error("unexpected column count from select");
		return matrix_job_ids;
	}
	
	// initialize result column structures
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = (void *) &my_job_id;
	result[0].is_unsigned = 0;
	result[0].is_null = 0;
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could nto bind parameters for select");
		return matrix_job_ids;
	}
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return matrix_job_ids;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return matrix_job_ids;
	}
	
	while (mysql_stmt_fetch(m_stmt) == 0) {  // fetch each row
		// store into vector
		matrix_job_ids.push_back(my_job_id);
	}
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return matrix_job_ids;
}


Matrixdata DBManager::get_matrix_job(const int& id) {
	Matrixdata matrix_data;

	char *stmt_str = "SELECT special_key FROM wkdd_matrix_jobs WHERE id = ?";
	MYSQL_BIND param[1];
	MYSQL_BIND result[1];
	int my_job_id;
	char my_special_key[45];
	unsigned long my_special_key_length;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return matrix_data;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 1) {
		print_stmt_error("unexpected column count from select");
		return matrix_data;
	}
	
	// initialize result column structures
	memset((void *)param, 0, sizeof(param));
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_job_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_key parameter
	result[0].buffer_type = MYSQL_TYPE_STRING;
	result[0].buffer = (void *) &my_special_key;
	result[0].buffer_length = sizeof(my_special_key);
	result[0].length = &my_special_key_length;
	result[0].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for select");
		return matrix_data;
	}
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could nto bind parameters for select");
		return matrix_data;
	}
	
	// set variable
	my_job_id = id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return matrix_data;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return matrix_data;
	}
	
	mysql_stmt_fetch(m_stmt);  // fetch the single result
	std::string key_str(my_special_key);
	matrix_data = Matrixdata(id, key_str);
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return matrix_data;
}

bool DBManager::user_email_exists(const int& id) {
	bool exists = false;

	char *stmt_str = "SELECT user_email FROM wkdd_matrix_jobs WHERE id = ?";
	MYSQL_BIND param[1];
	MYSQL_BIND result[1];
	int my_job_id;
	char my_user_email[45];
	my_bool is_null;
	unsigned long my_user_email_length;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return exists;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 1) {
		print_stmt_error("unexpected column count from select");
		return exists;
	}
	
	// initialize result column structures
	memset((void *)param, 0, sizeof(param));
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_job_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_key parameter
	result[0].buffer_type = MYSQL_TYPE_STRING;
	result[0].buffer = (void *) &my_user_email;
	result[0].buffer_length = sizeof(my_user_email);
	result[0].length = &my_user_email_length;
	result[0].is_null = &is_null;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for select");
		return exists;
	}
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could nto bind parameters for select");
		return exists;
	}
	
	// set variable
	my_job_id = id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return exists;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return exists;
	}
	
	mysql_stmt_fetch(m_stmt);  // fetch the single result if it exists
	if (!is_null) {
		exists = true;
	}

	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return exists;
}

