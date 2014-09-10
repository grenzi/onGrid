#!/usr/bin/python

import os
import MySQLdb

upload_path = "/matrix_temporary_uploads/"
result_path = "/matrix_temporary_results/"
db = MySQLdb.connect(host="localhost", user="user_name", passwd="password", db="db_name")
cur = db.cursor(MySQLdb.cursors.DictCursor)
sql = """SELECT id, special_key FROM wkdd_matrix_jobs WHERE user_email IS NULL AND end_time < NOW() - INTERVAL 1 WEEK"""
cur.execute(sql)
rows = cur.fetchall()

for row in rows:
	#delete file
	up_row_file = upload_path + row['special_key'] + "_row.txt"
	up_col_file = upload_path + row['special_key'] + "_col.txt"
	cont_file = result_path + row['special_key'] + "_contribution.txt"
	dist_file = result_path + row['special_key'] + "_distance.txt"
	if (os.path.exists(up_row_file)):
		os.unlink(up_row_file)
	if (os.path.exists(up_col_file)):
		os.unlink(up_col_file)
	if (os.path.exists(cont_file)):
		os.unlink(cont_file)
	if (os.path.exists(dist_file)):
		os.unlink(dist_file)
	#delete entry from database
	try:
		sql = """DELETE FROM wkdd_matrix_jobs WHERE id=%s""" 
		cur.execute(sql, (row['id']))
		db.commit()
	except Exception:
		db.rollback()

cur.close()
db.close()