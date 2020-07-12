<?php
class biblogin
{
    //private $endpoint = "https://auth.dbc.dk/oauth/token";
    private $endpoint = "https://login.bib.dk/oauth/token";

    public function setEndpoint($endpoint)
    {
        $this->endpoint = $endpoint;
    }
    public function setClientId($ClientId)
    {
        $this->ClientId = $ClientId;
    }
    public function setClientSecret()
    {
        $this->ClientSecret = $ClientSecret;
    }
    /*
    * gets a bib login access token using password grant.
    */
    public function getToken($username = null,$password= null,$libraryid = null)
    {
        //init curl
        $headers = array("Content-Type:application/x-www-form-urlencoded");
        $curl = curl_init();
            
            $postData = array();
            
            curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
            
            //curl_setopt($curl, CURLOPT_USERPWD, $this->ClientId . ":" . $this->ClientSecret);

            curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
            //do post
            curl_setopt($curl, CURLOPT_POST, true);
            if($username != null && $password != null && $libraryid != null)
            {
                curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query(array('grant_type' => 'password','username' => $username,
                'password' => $password,'agency' => $libraryid,'client_id' => $this->ClientId,'client_secret' => $this->ClientSecret)));
            }
            curl_setopt($curl, CURLOPT_URL, $this->endpoint);
            
            $response = curl_exec($curl);
            $data = json_decode($response);
            $data->expires = time()+$data->expires_in;
            return $data;
        
    }
    /*
    * Logout of the single sign on platform.
    */
    public function revokeToken($token)
    {
        curl_setopt($curl, CURLOPT_DELETE, true);
        curl_setopt($curl, CURLOPT_URL, "https://login.bib.dk/oauth/revoke?access_token=".$token);
        $response = curl_exec($curl);
        $data = json_decode($response);
        return $data;
    }
    /*
    * Is it possible to drop a cookie like the open platform does ?
    * to allow single-sign on without going throu single-sign on.
    */
    private function DropCookieHack()
    {

    }
    /*
    * Helper function to check if a user is still logged in with the bib login sso platform.
    */
    public static function isloggedin()
    {
        global $user;
        if(user_is_logged_in() && isset($_SESSION['bib_login_token']))
        {
            $tokendata = $_SESSION['bib_login_token'];

            //is token exired ?
            
            $current_time = time();
            if($tokendata->expires <= $current_time)
            {
                return false;
            }
            else 
            {
                return $tokendata;
            }
        }
        else
        {
            return false;
        }
    }
}

