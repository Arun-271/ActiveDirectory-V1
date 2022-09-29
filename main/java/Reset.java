

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class Reset
 */
@WebServlet("/Reset")
public class Reset extends HttpServlet {
	private static final long serialVersionUID = 1L;
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

		String accountName = request.getParameter("accountName");
		String newPassword = request.getParameter("newPassword");
		NativeMethods obj = new NativeMethods();
		int result = obj.resetPassword(accountName, newPassword);		
		PrintWriter out = response.getWriter();
		if(result==0) {
			out.print("<div>Password Updated successfully</div>");
		}
		else {
			out.print("<div>Password not updated (Error occured)</div>");
		}
	}

}
