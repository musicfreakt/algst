<%@ page import="util.LocaleUtil" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<article>
	<div class = "article-content">
		<form action="/social/" method="post"> 
		    <input type="text" placeholder='<%=LocaleUtil.getString("name")%>' name="username">
		    <br> 
		    <textarea name="text" placeholder='<%=LocaleUtil.getString("text")%>' rows="3"></textarea>
		    <br>
		    <hr>
		    <button id="post-btn" type="submit"><%=LocaleUtil.getString("submit")%></button> 
		</form>
	</div>
</article>