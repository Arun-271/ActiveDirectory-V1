

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONArray;

@WebServlet("/UserAccountDetails")
public class UserAccountDetails extends HttpServlet {
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String user = request.getParameter("user");
		NativeMethods obj = new NativeMethods();
		ArrayList<String>res = obj.userAccountDetailsEdited(user);
		PrintWriter out = response.getWriter();
		JSONArray jsArray = new JSONArray(res);
		out.print(jsArray);
	}

}
