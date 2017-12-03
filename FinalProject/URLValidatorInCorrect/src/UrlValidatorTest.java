/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 *  * this work for additional information regarding copyright ownership.
 *   * The ASF licenses this file to You under the Apache License, Version 2.0
 *    * (the "License"); you may not use this file except in compliance with
 *     * the License.  You may obtain a copy of the License at
 *      *
 *       *      http://www.apache.org/licenses/LICENSE-2.0
 *        *
 *         * Unless required by applicable law or agreed to in writing, software
 *          * distributed under the License is distributed on an "AS IS" BASIS,
 *           * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *            * See the License for the specific language governing permissions and
 *             * limitations under the License.
 *              */


import junit.framework.TestCase;
import java.util.Random;
/**
 *  * Performs Validation Test for url validations.
 *   *
 *    * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 *     */
public class UrlValidatorTest extends TestCase {

   private boolean printStatus = false;
   private boolean printSuccess = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   public UrlValidatorTest(String testName) {
      super(testName);
   }

  
   public void testManualTest()
   {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   printPassFail(urlVal, "http://www.amazon.com", true);
	   printPassFail(urlVal, "https://www.wikipedia.org", true);
	   printPassFail(urlVal, "http://localhost/", false);
	   printPassFail(urlVal, "http:\\randomtestsite.com", false);
	   printPassFail(urlVal, "ftp://testsite.org/sample.txt", true);
	   printPassFail(urlVal, "http://", false);
	   printPassFail(urlVal, "http://.www.foo.bar/", false);
	   printPassFail(urlVal, "a5d://test.com", true);
	   printPassFail(urlVal, "http:/asdf.org", false);
	   printPassFail(urlVal, "http://12.23.45.67.89", false);
	   printPassFail(urlVal, "http://foo.bar/?state=California&city=Oakland#bar", true);
	   printPassFail(urlVal, "http://123.com/?z=abc&y=jkl", true);
	   printPassFail(urlVal, "ftp://foo.bar/?q=testing&p=demo", false);
	   printPassFail(urlVal, "ftp://foo.bar/?q=testing", false);
	   printPassFail(urlVal, "abc.123.testing", false);
	   printPassFail(urlVal, "http:///foobar.org/testing", false);
	   printPassFail(urlVal, "http://a.b-c.de", true);
	   printPassFail(urlVal, "http://1234567", false);
	   printPassFail(urlVal, "http://125.2.5.12:63a", false);
	   printPassFail(urlVal, "https://www.wikipedia.org//animals//dog", false);
	   printPassFail(urlVal, "http://123corpration.net/..///asdf", false);
   }
   
   
   public void testYourFirstPartition()
   {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_LOCAL_URLS);
	   printPassFail(urlVal, "http://localhost/", true);
	   printPassFail(urlVal, "http://localhost", true);
   }
   
   public void testYourSecondPartition(){
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_2_SLASHES);
	   printPassFail(urlVal, "http://fakecompany.com/testing//demo//software", true);
   }
   
   public void testYourThirdPartition() {
	   UrlValidator urlVal = new UrlValidator();
	   printPassFail(urlVal, "http://123corp.com//testPath", false);
	   printPassFail(urlVal, "a2t://test.com", false);
	   printPassFail(urlVal, "http://test.com", true);
	   printPassFail(urlVal, "https://test.com", true);
	   printPassFail(urlVal, "ftp://test.com", true);
	   printPassFail(urlVal, "http://test.com#frag", true);
   }
   
   public void testYourFourthPartition() {
	   UrlValidator urlVal = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES);
	   printPassFail(urlVal, "a2t://test.com", true);
	   printPassFail(urlVal, "foobar://abc123.com", true);
	   printPassFail(urlVal, "http://test.com", true);
	   printPassFail(urlVal, "https://test.com", true);
	   printPassFail(urlVal, "ftp://test.com", true);
   }
   
   public void testYourFifthPartition() {
	   UrlValidator urlVal = new UrlValidator(UrlValidator.NO_FRAGMENTS);
	   printPassFail(urlVal, "http://test.com#frag", false);
	   printPassFail(urlVal, "http://test.com/randompath#frag", false);
     printPassFail(urlVal, "http://test.com", true);
   }
  
  public void testCountryPartition() {
	   UrlValidator urlVal = new UrlValidator();
	   System.out.println("Testing country codes Norway .no");
	   printPassFail(urlVal, "https://www.norid.no/en/", true);
	   printPassFail(urlVal, "https://www.regjeringen.no/en/aktuelt/a-budget-for-economic-growth-job-creation-and-a-sustainable-welfare-state/id2574821/", true);
	   printPassFail(urlVal, "http://www.miljodirektoratet.no/en/Areas-of-activity1/Outdoor-recreation/Right-to-Roam/", true);
	   printPassFail(urlVal, "https://www.visitnorway.com/places-to-go/fjord-norway/bergen/listings-bergen/full-package-fjord-tour-from-voss/10375/", true);
	   printPassFail(urlVal, "http://www.norwayactive.no/?utm_source=visitnorway.com&utm_medium=listing&utm_content=visit-website-link", true);
	
   }
   
   
   public void testIsValid()
   {
     /*Dont forget to add the import that I placed at the top: import java.util.Random;*/
 /*The test will be a random test where the parts of the url are pulled and tested. All parts are valid so every combination should return valid */
	   Random rand = new Random();
	   
	   /* Create a string array for each of the url parts to have the random test choose from */
	   String[] testUrlScheme = {"http://","ftp://","h3t://", "https://"};
	   String[] testUrlAuthority = {"www.google.com","go.com", "go.au", "0.0.0.0"};
	   String[] testUrlPort = {":80", "", ":65535"};
	   String[] testUrlPath = {"/test1","/$23", "/test1/","","/test1/file"};
	   String[] testUrlQuery = {"?action=view","", "?x=test&y=abc"};
	   
	   /*Initialize the variables*/
	   int r;
	   String urlTest = "";
	   
	   /*Create the validator*/
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   
	   System.out.println("---------------BEGIN ALL VALID TESTING------------------");
	   /*Loop 10,000 times*/
	   for(int i = 0;i<10000;i++)
	   {
		   /*Reset the url to test*/
		   urlTest = "";
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlScheme.length);
		   urlTest += testUrlScheme[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlAuthority.length);
		   urlTest += testUrlAuthority[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlPort.length);
		   urlTest += testUrlPort[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlPath.length);
		   urlTest += testUrlPath[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlQuery.length);
		   urlTest += testUrlQuery[r];
		   
		   /*Print the result of the test*/
		   printPassFail(urlVal, urlTest, true);
	   }
	   System.out.println("---------------END ALL VALID TESTING------------------");
	   
	   System.out.println("---------------BEGIN ALL INVALID TESTING AND NO TESTS SHOULD PRINT------------------");
	   
	   /*The test will be a random test where the parts of the url are pulled and tested. All parts are invalid so every combination should return invalid */
	   
	   /* Create a string array for each of the url parts to have the random test choose from */
	   String[] testUrlSchemeIn = {"http:/","ftp:\\",":","//", "https:///"};
	   String[] testUrlAuthorityIn = {"wwwgooglecom",".aa", "go/test", "invalid,test", "0.-1.-1.-1"};
	   String[] testUrlPortIn = {":a", ":1028938",":-1", "283abd"};
	   String[] testUrlPathIn = {"//test1", "test1/","/test1.file"};
	   String[] testUrlQueryIn = {"query","@test"};
	   
	   /*Loop 10,000 times*/
	   for(int i = 0;i<10000;i++)
	   {
		   /*Reset the url to test*/
		   urlTest = "";
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlSchemeIn.length);
		   urlTest += testUrlSchemeIn[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlAuthorityIn.length);
		   urlTest += testUrlAuthorityIn[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlPortIn.length);
		   urlTest += testUrlPortIn[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlPathIn.length);
		   urlTest += testUrlPathIn[r];
		   
		   /*Randomly select an integer based on the url part length and create a url out of it */
		   r = rand.nextInt(testUrlQueryIn.length);
		   urlTest += testUrlQueryIn[r];
		   
		   /*Print the result of the test*/
		   printPassFail(urlVal, urlTest, false);
	   }
	   System.out.println("---------------END ALL INVALID TESTING------------------");
   }
   

   /**
 *     * Create set of tests by taking the testUrlXXX arrays and
 *         * running through all possible permutations of their combinations.
 *             *
 *                 * @param testObjects Used to create a url.
 *                     */

   private void printPassFail(UrlValidator urlVal, String url, boolean expected) {
	   String validity, output;
	   boolean actual = urlVal.isValid(url);
	   if (actual) {
		   validity = "valid";
	   } else {
		   validity = "invalid";
	   }
	   output = url + " -- result: " + validity;
	   if (actual == expected) {
		   if (printSuccess) {
			   System.out.println(output + "      ----- SUCCESS -----");
		   }
	   } else {
		   System.out.println(output + "      ----- FAILED ----- ");
	   }
   }

}
