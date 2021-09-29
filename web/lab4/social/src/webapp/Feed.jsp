<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
 
<% 
    if(request.getParameter("username") != null) { 
        out.println("<article>"+ 
                        "<h2 class = 'username'>" + request.getParameter("username") + "</h2>" + 
                        "<div class = 'date'>" + LocaleUtil.getTime() + "</div>" +
                        "<p class = 'post_text'>" + request.getParameter("text") + "</p><hr>"+ 
                        "<div class = 'likes'>0</div>" +
                    "</article>"); 
    } 
%>
<article>
	<h2 class = "username">kivyfreakt</h2>
	<div class = "date"><%= LocaleUtil.getTime() %></div>
	<p class = "post_text">Hello World!</p>
	<hr>
	<div class = "likes">2</div>
</article>
<article>
	<h2 class = "username">asscar</h2>
	<div class = "date"><%= LocaleUtil.getTime() %></div>
	<p class = "post_text">leninapaket</p>
	<hr>
	<div class = "likes">5</div>
</article>
