

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


@WebServlet("/Delete")
public class Delete extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String accountName = request.getParameter("accountName");

		NativeMethods obj = new NativeMethods();
		int result = obj.deleteUser(accountName);
		
		PrintWriter out = response.getWriter();
		if(result==0) {
			out.print("<div>User deleted successfully</div>");
		}
		else {
			out.print("<div>Error occured</div>");
		}
	}

}
