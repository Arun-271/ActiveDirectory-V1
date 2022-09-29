import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.TimeZone;

public class NativeMethods {
	 static{
	       System.loadLibrary("testNativeMethods");
	    }
	    public native int createUser(String firstName , String lastName , String accountName,String intialPassword,
	    		String descp , String email , String street,String city,
	    		String state , String country , String postalCode,String telephone,
	    		String mobile , String title , String department,String company,
	    		String manager );
	    public native int modifyUser(String account,String firstName ,String lastName);
	    public native int deleteUser(String account);
	    public native int resetPassword(String account , String newPassword);
	    public native String[] ListAllUserFromADs();
	    public native byte[] profilePic(String user);
	    // View UserDetails By tab;
	    public native String[] userGeneralDetails(String user);
	    public native String[] userAddresDetails(String user);
	    // Account tab;
	    public native String[] userAccountDetails(String user);
	    public native long passWordLastSet(String user);
	    public native long accountExpires(String user);
		public native byte[] LoginHours(String user);
		public native String[] directReports(String user);
	    public ArrayList<String> userAccountDetailsEdited(String user){
	    	String [] res =userAccountDetails(user);
	    	ArrayList<String> al =
	                new ArrayList<String>(Arrays.asList(res));
	    	long res2= passWordLastSet(user);
	    	long res3= accountExpires(user);
	    	String pwdDate = convertEpochToTime(res2);
	    	String expireDate = convertEpochToTime(res3);
	    	al.add(pwdDate);
	    	al.add(expireDate);
	    	return al;
	    }
	    public String convertEpochToTime(long value){
	        long nanoseconds = value;

	        long mills = (nanoseconds/10000000);
	        long unix = (((1970-1601) * 365) - 3 + Math.round((1970-1601)/4) ) * 86400L;
	        long timeStamp = mills - unix ;
	    
	     Date date = new Date(timeStamp*1000L); // *1000 is to convert seconds to milliseconds
	     
	     SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss z"); // the format of your date

	    sdf.setTimeZone(TimeZone.getTimeZone("America/Tijuana")); // give a timezone reference for formating (see comment at the bottom

	     String formattedDate = sdf.format(date);
	     return formattedDate;
	    }
	    // Telephone tab
	    public native String[] telephone(String user);
	    //organization details
	    public native String[] organizationDetails(String user);
}
