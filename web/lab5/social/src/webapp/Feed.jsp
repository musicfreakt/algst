<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
 
<% 
	// request.getParameter("username")
	String username = (String)session.getAttribute("username");
	String post = request.getParameter("text");
    if(username != null && post != null) { 
        out.println("<article><div class = 'article-content'>"+ 
                        "<h2 class = 'username'>" + username + "</h2>" + 
                        "<div class = 'date'>" + LocaleUtil.getTime() + "</div>" +
                        "<div class = 'post-text'>" + post + "</div><hr>"+ 
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
