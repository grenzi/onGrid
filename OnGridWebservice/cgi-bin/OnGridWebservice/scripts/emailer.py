#!/usr/bin/python

import sys
import MySQLdb
import smtplib

from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText

job_id = sys.argv[1]
db = MySQLdb.connect(host="localhost", user="user_name", passwd="password", db="db_name")
cur = db.cursor(MySQLdb.cursors.DictCursor)
sql = """SELECT wkdd_matrix_jobs.job_name, wkdd_matrix_jobs.user_email, wkdd_users.firstname FROM wkdd_matrix_jobs JOIN wkdd_users ON wkdd_matrix_jobs.user_email = wkdd_users.email WHERE wkdd_matrix_jobs.id=%s"""
affected_count = cur.execute(sql, (job_id))

if affected_count > 0:

	row = cur.fetchone()
	user_name = row['firstname']
	job_name = row['job_name']
	receiver = row['user_email']
	sender = "my_email"
	
	# Create message container - the correct MIME type is multipart/alternative.
	msg = MIMEMultipart('alternative')
	msg['Subject'] = "Job Completed."
	msg['From'] = sender
	msg['To'] = receiver

	# Create the body of the message (a plain-text and an HTML version).
	text = "Hello " + user_name + ",\n\nYour job '" + job_name + "' has completed. Please use your access code to download your results at my_homepage."
	html = """\
	<html>
	  <head></head>
	  <body>
	    <p>Hello """ + user_name + """,<br /><br />Your job '""" + job_name + """' has completed. Please use your access code to download your results at <a href="my_homepage">my_homepage</a>.
	    </p>
	  </body>
	</html>
	"""

	# Record the MIME types of both parts - text/plain and text/html.
	part1 = MIMEText(text, 'plain')
	part2 = MIMEText(html, 'html')

	# According to RFC 2046, the last part of a multipart message, in this case
	# the HTML message, is best and preferred.
	msg.attach(part1)
	msg.attach(part2)

	s = smtplib.SMTP('localhost')
	s.sendmail(sender, receiver, msg.as_string())
	s.quit()

cur.close()
db.close()