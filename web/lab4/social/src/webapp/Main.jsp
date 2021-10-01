<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" errorPage="/Error.jsp"%>
<!DOCTYPE html>
<html>
<% 
	request.setCharacterEncoding("UTF-8"); 
	
	String text = request.getParameter("text"); 
	if (text != null && text.length() > 140) 
		throw new IllegalArgumentException(); 
	
	String lang = request.getParameter("lang"); 
	
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
		<div id="header-content">
			<div id="header">
				<h1><%=LocaleUtil.getString("header")%></h1>
			</div>
			<div id="locale">
				<a href='/social/?lang=ru'>ru</a>
				/
	       		<a href='/social/?lang=en'>en</a>
	       	</div>
       	</div>
    </header>
	<main>
		<%@include file="Post.jsp"%> 
		<%@include file="Feed.jsp"%> 
	</main>
</body>
</html>