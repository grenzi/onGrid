This should be performed after generating the graph and vertex cui/name files.
The two output files correspond to MySQL tables, and can be loaded via: mysql> LOAD DATA LOCAL INFILE '/path/file.txt' INTO TABLE mytable;
Insert these into wkdd_connected_labelsxref and wkdd_isolated_labels.

To access mysql command shell: mysql --host=localhost --user=myname --password=mypass mydb