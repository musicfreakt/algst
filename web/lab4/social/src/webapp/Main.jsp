<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<% 
	request.setCharacterEncoding("UTF-8"); 
	// Чтение параметров из строки 
	String lang = request.getParameter("lang"); 
	// установка локализации через LocaleManager 
	if (!LocaleUtil.setup(lang)) { 
	   	response.sendError(HttpServletResponse.SC_NOT_ACCEPTABLE, "Приложение не поддерживает запрашиваемый язык"); 
		return; 
	}
%> 
<head>
<meta charset="UTF-8">
<link rel="stylesheet" href="css/style.css">
<title><%=LocaleUtil.getString("title")%></title>
</head>
<body>
	<header>
		<h1 id="header"><%=LocaleUtil.getString("header")%></h1>
		<div id="locale">
			<a href='/social/?lang=ru'>ru</a>
			/
       		<a href='/social/?lang=en'>en</a>
       	</div>
    </header>
	<main>
		<div id = "btn">
			<a id="post_btn" href="/social/post" ><%=LocaleUtil.getString("post")%></a>
		</div>
		<%@include file="Feed.jsp"%> 
	</main>
</body>
</html>