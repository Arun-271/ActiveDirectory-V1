

import java.io.IOException;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.text.DateFormatSymbols;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONArray;

/**
 * Servlet implementation class LogonHours
 */
@WebServlet("/LogonHours")
public class LogonHours extends HttpServlet {
	private static final long serialVersionUID = 1L;
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String user = request.getParameter("user");
		NativeMethods obj = new NativeMethods();
		JSONArray data;
		try {
			byte[] res = obj.LoginHours(user);
			final String[] DAYS = DateFormatSymbols.getInstance().getWeekdays();    
	        BigInteger bi = new BigInteger(res);
	        ArrayList <String> da = new ArrayList<String>();
	        for (int day = 0, bit = 24 * 7 - 1; day < 7; day++) {
	           String s = " Hours allowed for " + DAYS[day+1];
	           da.add(s);
	            //System.out.printf(" Hours allowed for %s%n", DAYS[day + 1]);
	            for (int hour = 0; hour < 24; hour++, bit--) {
	                boolean allowed = bi.testBit(bit);
	                String r =  " Login permitted for hour " + hour + " " + allowed;
	                da.add(r);
	                // System.out.printf("\tLogin permitted for hour %d?: %b%n", hour, allowed);
	            }
	        }
	        JSONArray jsArray = new JSONArray(da);
	        PrintWriter out = response.getWriter();
		    out.print(jsArray);
		}catch(Exception e){
			ArrayList <String> da = new ArrayList<String>();
			da.add("Login hours not set");
			JSONArray jsArray = new JSONArray(da);
	        PrintWriter out = response.getWriter();
		    out.print(jsArray);
		}
		
	}
}
