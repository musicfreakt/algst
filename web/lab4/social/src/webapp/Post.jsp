<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8"> 
    <title><%=LocaleUtil.getString("post")%></title> 
</head>
<body> 
<form action="/social/" method="post"> 
    <label><%=LocaleUtil.getString("name")%>:
        <input type="text" name="username"><br> 
    </label>
    <label><%=LocaleUtil.getString("text")%>:
        <input type="text" name="text"><br> 
    </label>
    <button type="submit"><%=LocaleUtil.getString("submit")%></button> 
</form> 
</body> 
</html>