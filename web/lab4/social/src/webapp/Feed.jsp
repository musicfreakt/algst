<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
 
<% 
    if(request.getParameter("username") != null && request.getParameter("username") != "") { 
        out.println("<article><div class = 'article-content'>"+ 
                        "<h2 class = 'username'>" + request.getParameter("username") + "</h2>" + 
                        "<div class = 'date'>" + LocaleUtil.getTime() + "</div>" +
                        "<div class = 'post-text'>" + request.getParameter("text") + "</div><hr>"+ 
                        "<div class = 'likes'>" + LocaleUtil.getString("likes") + ": 0</div>" +
                    "</div></article>"); 
    } 
%>

<article>
	<div class = "article-content">
		<h2 class = "username">asscar</h2>
		<div class = "date"><%=LocaleUtil.getTime()%></div>
		<div class = "post-text">leninapaket</div>
		<hr>
		<div class = "likes"><%=LocaleUtil.getString("likes")%>: 5</div>
	</div>
</article>
<article>
	<div class = "article-content">
		<h2 class = "username">kivyfreakt</h2>
		<div class = "date"><%= LocaleUtil.getTime() %></div>
		<div class = "post-text">Hello World!</div>
		<hr>
		<div class = "likes"><%= LocaleUtil.getString("likes") %>: 2</div>
	</div>
</article>
