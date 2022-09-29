

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONArray;

@WebServlet("/ListAllUsers")
public class ListAllUsers extends HttpServlet {
	private static final long serialVersionUID = 1L;

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		NativeMethods obj = new NativeMethods();
		String[] res = obj.ListAllUserFromADs();
		System.out.println("Camer her");
		PrintWriter out = response.getWriter();
		JSONArray jsArray = new JSONArray(res);
		out.print(jsArray);
	}

}
