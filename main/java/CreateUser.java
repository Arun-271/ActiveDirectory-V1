import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

@WebServlet("/CreateUser")
public class CreateUser extends HttpServlet {

	private static final long serialVersionUID = 1L;

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String firstName = request.getParameter("firstName");
		String lastName= request.getParameter("lastName");
		String accountName = request.getParameter("accountName");
		String intitalPassword = request.getParameter("intialPassword");
		String desp = request.getParameter("desc");
		String mail = request.getParameter("mail");
		String street = request.getParameter("street");
		String city = request.getParameter("city");
		String state = request.getParameter("state");
		String country = request.getParameter("country");
		String postalCode = request.getParameter("postalCode");
		String telephone = request.getParameter("telephone");
		String mobile= request.getParameter("mobile");
		String jobtitle= request.getParameter("title");
		String department= request.getParameter("department");
		String company = request.getParameter("company");
		String manager = request.getParameter("manager");
		NativeMethods obj = new NativeMethods();
		int result = obj.createUser(firstName, lastName, accountName, intitalPassword,desp,mail,
				street,city,state,country,postalCode,telephone,mobile,jobtitle,department,
				company,manager
				);
		PrintWriter out = response.getWriter();
		if(result==0) {
			out.print("<div>User Created Successfully</div>");
		}
		else {
			out.print("<div>User not created</div>");
		}
	}

}
