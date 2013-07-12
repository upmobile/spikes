require 'oauth'
require 'json'
require 'rexml/document'
include REXML # Se incluye esta linea para que no se tenga que poner el prefijo REXML::

def prepare_access_token(oauth_token, oauth_token_secret)
  consumer = OAuth::Consumer.new("ZIaUkHC2dqpBAxUiUU9w", "eO0t8SYwZxrGuYoEC7MJdMbLakMqPMH7vpnj1uC1NM", { :site => "http://api.twitter.com", :scheme => :header})

  # now create the access token object from passed values
  token_hash = { :oauth_token => oauth_token, :oauth_token_secret => oauth_token_secret }
  access_token = OAuth::AccessToken.from_hash(consumer, token_hash )

  return access_token
end

## Exchange our oauth_token and oauth_token secret for the AccessToken instance.

access_token = prepare_access_token("323319782-jZmmsxWTkLKfCEnLkj5YDoOD6pjredWwfXIF2hOF", "XfmAQlgHb7ty76MfQnUsEH64QHJdmAjd78RSRHmMI")


#Obtienes el timeline de la cuenta Twitter
#response = access_token.request(:get, "http://api.twitter.com/1/statuses/home_timeline.json")

#Escribir en Twitter
#response = access_token.request(:post, "http://api.twitter.com/1/statuses/update.json?status=Trying+to+use+Streaming+API")
#response = access_token.request(:post, "http://stream.twitter.com/1/statuses/update.json?status=Using+Streaming+API")

#Obtener los twitters que se escribieron
response = access_token.request(:get, "http://api.twitter.com/1/statuses/user_timeline.json?screen_name=mic_user2")

result = JSON.parse(response.body)

puts "El usuario user2_mic escribio los siguientes Twitters"
i=0
result.each { |status|
  i+=1
  puts "Twitter #{i}: #{status['text']}"
}