<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title><%=LocaleUtil.getString("error")%></title>
</head>
<body>
	<h1><%=LocaleUtil.getString("error")%></h1> 
	<%=LocaleUtil.getString("error.msg")%> 
	<br>
	<a href="/social"><%=LocaleUtil.getString("title")%> </a>
</body>
</html>