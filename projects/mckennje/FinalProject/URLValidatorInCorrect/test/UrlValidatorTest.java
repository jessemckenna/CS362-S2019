/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import junit.framework.TestCase;

/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision$
 */
public class UrlValidatorTest extends TestCase {

//   private final boolean printStatus = false;
//   private final boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   public UrlValidatorTest(String testName) {
      super(testName);
   }

	//////////////////////////////////
	//Final Project PartB Unit Tests//
	//////////////////////////////////
	
	public void testIsValidAgain() {
		String url = "";
		boolean expect = false;
		boolean result = false;
		
		UrlValidator urlVal = new UrlValidator();

		//Loops to create all possible test urls
		for (int i = 0; i < testUrlScheme.length; i++) {
			for (int j = 0; j < testUrlAuth.length; j++) {
				for (int k = 0; k < testUrlPath.length; k++) {
					for (int l = 0; l < testUrlQuery.length; l++) {
						url = testUrlScheme[i].item
								+ testUrlAuth[j].item
								+ testUrlPath[k].item
								+ testUrlQuery[l].item;

						// If all components are valid, expect URL is valid;
						// if any component is invalid, expect URL is invalid.
						expect = testUrlScheme[i].valid
									&& testUrlAuth[j].valid
									&& testUrlPath[k].valid
									&& testUrlQuery[l].valid;
						result = urlVal.isValid(url);
						assertEquals(url, expect, result);
					}
				}
			}
		}
	}

	//////////////////////////////////
	//URL parts for test data//
	//////////////////////////////////
	ResultPair[] testUrlScheme = {new ResultPair("http://", true),
								new ResultPair("https://", true),
								new ResultPair("http:/", false),
								new ResultPair("http:", false),
								new ResultPair("http/", false),
								new ResultPair("https:/", false),
								new ResultPair("https:", false),
								new ResultPair("https/", false)};
	
	ResultPair[] testUrlAuth = {new ResultPair("www.google.com", true),
								new ResultPair("www.usa.gov", true),
								new ResultPair("www.npr.org", true),
								new ResultPair("", false),
								new ResultPair("test", false),
								new ResultPair("test.", false),	
								new ResultPair(".test", false),
								new ResultPair("0.0.0.0", true),
								new ResultPair("255.255.255.255", true),
								new ResultPair(".0.0.0.0", false),
								new ResultPair("256.256.256.256", false),
								new ResultPair("test.au", true),
								new ResultPair("www.google.com:80", true),
								new ResultPair("www.google.com:65535", true),
								new ResultPair("www.google.com:65536", false)};
	
	ResultPair[] testUrlPath = {new ResultPair("/test", true),
								new ResultPair("/test1", true),
								new ResultPair("/test123", true),
								new ResultPair("/test/test", true),
								new ResultPair("/test/test1", true),
								new ResultPair("/test123", true),
								new ResultPair("/..", false),
								new ResultPair("/test/", true),
								new ResultPair("/test//", false),
								new ResultPair("/test//test", false)};
	
	ResultPair[] testUrlQuery = {new ResultPair("?action=view", true),
								new ResultPair("?action=edit&mode=up", true),
								new ResultPair("?q=1;q=2;q=3", false),
								new ResultPair("?q=1+q=2+q=3", true)};

}
