<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd">
<html>
	<head>
		<link rel="shortcut icon" href="favicon.ico" type="image/x-icon" />
		<link rel="stylesheet" type="text/css" href="./styles/global-styles.css" />
	</head>
	<body>
		<div>&nbsp;</div>
		<div class="page-title">Setting up the database...</div><br /><br />
		<span style="text-align: center;">
			<div class="label">If you see no error messages, it should be done.</div>
			<div>&nbsp;</div>
			<div class="label"><a href="index.php">Continue back to the frontpage.</a></div>
		</span>
		<br />
		<script>
			try{
				window.sessionStorage.clear();
				window.localStorage.clear();
			}catch(e){
				alert("Error clearing HTML 5 Local and Session Storage" + e.toString());
			};
		</script>
		<div class="database-success-message">HTML 5 Local and Session Storage cleared unless error popped-up already.</div>
<div class="database-informative-message">Attempting to connect to MySQL server on host localhost with user name mutillidae</div><div class="database-informative-message">Connected to MySQL server at localhost as mutillidae</div><div class="database-informative-message">Preparing to drop database nowasp</div><div class="database-success-message">Executed query 'DROP DATABASE IF EXISTS' for database nowasp with result 1</div><div class="database-informative-message">Preparing to create database nowasp</div><div class="database-success-message">Executed query 'CREATE DATABASE' for database nowasp with result 1</div><div class="database-informative-message">Switching to use database nowasp</div><div class="database-informative-message">Executed query 'USE DATABASE' nowasp with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE TABLE' with result 1</div><div class="database-success-message">Executed query 'INSERT INTO TABLE' with result 1</div><div class="database-success-message">Executed query 'CREATE PROCEDURE' with result 1</div><div class="database-success-message">Executed query 'CREATE PROCEDURE' with result 1</div><div class="database-success-message">Executed query 'CREATE PROCEDURE' with result 1</div><div class="database-informative-message">Trying to build XML version of accounts table to update accounts XML data/accounts.xml</div><div class="database-informative-message">Do not worry. A default version of the file is included if this does not work.</div><div class="database-informative-message">Trying to build text version of accounts table to update password text file passwords/accounts.txt</div><div class="database-informative-message">Do not worry. A default version of the file is included if this does not work.</div><div class="database-success-message">Executed query 'SELECT * FROM accounts'. Found 24 records.</div><div class="database-success-message">Wrote accounts to data/accounts.xml</div><div class="database-success-message">Wrote accounts to passwords/accounts.txt</div><script>if(confirm("No PHP or MySQL errors were detected when resetting the database.\n\nClick OK to proceed to http://bwa/mutillidae/?popUpNotificationCode=SUD1 or Cancel to stay on this page.")){document.location="http://bwa/mutillidae/?popUpNotificationCode=SUD1"};</script>	</body>
</html>