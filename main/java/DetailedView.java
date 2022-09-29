

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONArray;


@WebServlet("/DetailedView")
public class DetailedView extends HttpServlet {
	private static final long serialVersionUID = 1L;

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String user = request.getParameter("user");
		//System.out.println(user);
		NativeMethods obj = new NativeMethods();
		String[] res = obj.organizationDetails(user);
		PrintWriter out = response.getWriter();
		JSONArray jsArray = new JSONArray(res);
		out.print(jsArray);
	}
	
	

}
