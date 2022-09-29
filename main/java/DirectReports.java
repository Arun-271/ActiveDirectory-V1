

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONArray;

/**
 * Servlet implementation class DirectReports
 */
@WebServlet("/DirectReports")
public class DirectReports extends HttpServlet {
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String user = request.getParameter("user");
		NativeMethods obj = new NativeMethods();
		String arr[]  = obj.directReports(user);
//		System.out.println(arr.length);
//		ArrayList<String> res = new ArrayList<String>();
//		if(arr.length ==0) {
//			res.add("No Direct reports found");
//		}else {
//			for(int i = 0;i < arr.length;i++) {
//				res.add(arr[i]);
//			}
//		}
		PrintWriter out = response.getWriter();
		JSONArray jsArray = new JSONArray(arr);
		out.print(jsArray);
	}

}
