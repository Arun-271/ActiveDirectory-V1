

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.reflect.Array;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;

import javax.servlet.Servlet;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class ProfilePic
 */
@WebServlet("/ProfilePic")
public class ProfilePic extends HttpServlet {
	private static final long serialVersionUID = 1L;

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String user = request.getParameter("user");
		NativeMethods obj = new NativeMethods();
		byte[] res = obj.profilePic(user);
	    Charset csets = Charset.forName("UTF-8");
	    ByteBuffer bb=ByteBuffer.wrap(res);
	    csets.decode(bb);
	    bb.rewind();
	    byte[] array1=bb.array();
	    try{
	    FileOutputStream fos=new FileOutputStream("C:\\Arun\\apache-tomcat-9.0.65\\webapps\\ActiveDirectoryV2\\images\\"+user+".jpg");
	    fos.write(array1);
	    fos.flush();
	    fos.close();
	    PrintWriter out = response.getWriter();
	    out.write("C:/Arun/Eclipse workspace/ActiveDirectoryV2/src/main/webapp/images/profile.jpg");
	    }
	    catch(Exception e){
	        System.out.println(e.getMessage());
	        System.out.println(e.getStackTrace());
	    }
	}

}
