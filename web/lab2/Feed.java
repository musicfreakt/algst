package web;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Date;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class Feed
 */
@WebServlet(name = "feedServlet", value = "/feed")
public class Feed extends HttpServlet {
	private static final long serialVersionUID = 1L;

    /**
     * Servlet initialization
     */
    public void init() {
        System.out.println("Init the Posts servlet");
    }

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code> methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        // Задание типа кодировки для параметров запроса
        request.setCharacterEncoding("utf-8");
        // Чтение параметра name из запроса
        String username = request.getParameter("username");
        String post = request.getParameter("post");
        // Задание типа содержимого для ответа (в том числе кодировки)
        response.setContentType("text/html;charset=UTF-8");
        // Получение потока для вывода ответа
        PrintWriter out = response.getWriter();

        try {
            // Создание HTML-страницы
            out.println("<html>");
            out.println("<head><title>MAIN</title></head>");
            out.println("<body>");
            out.println("<h1 align='center'>Posts</h1>");
            out.println("<table align='center'>");
            out.println("<tr><td><b>Username</b></td><td><b>Text " +
                    "</b></td><td><b>Date</b></td><td><b>Likes</b></td></tr>");
            out.println("<tr><td>kivyfreakt</td><td>Hello World!" +
                    "</td><td>" + new Date() + "</td><td>1</td></tr>");
            out.println("<tr><td>asscar</td><td>leninapaket" +
                    "</td><td>" + new Date() + "</td><td>0</td></tr>");
            if (username != null && post != null) {
                out.println("<tr><td>" + username + "</td><td>" + post +
                        "</td><td>" + new Date() + "</td><td>0</td></tr>");
            }
            out.println("</table>");
            out.println("</body>");
            out.println("</html>");
        } finally {
            // Закрытие потока вывода
            out.close();
        }
    }

    /**
     * Handles the HTTP
     * <code>GET</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP
     * <code>POST</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Destroying the servlet
     */
    public void destroy() {
        System.out.println("Destroy the Posts servlet");
    }
}