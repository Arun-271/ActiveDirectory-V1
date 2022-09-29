

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class Update
 */
@WebServlet("/Update")
public class Update extends HttpServlet {
	private static final long serialVersionUID = 1L;

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String firstName = request.getParameter("firstName");
		String lastName= request.getParameter("lastName");
		String accountName = request.getParameter("accountName");
		NativeMethods obj = new NativeMethods();
		int result = obj.modifyUser(accountName, firstName, lastName);
		
		PrintWriter out = response.getWriter();
		if(result==0) {
			out.print("<div>User updated Successfully</div>");
		}
		else {
			out.print("<div>User not updated</div>");
		}
	}

}
