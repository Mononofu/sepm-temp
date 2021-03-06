require 'net/http'
require 'rexml/document'

# Web search for "madonna"
url = 'http://sepm.furidamu.org/cc.xml'


SCHEDULER.every '10s', :first_in => 0 do |job|
  # get the XML data as a string
  xml_data = Net::HTTP.get_response(URI.parse(url)).body

  # extract event information
  doc = REXML::Document.new(xml_data)
  status = doc.elements["//Project[@name='sepm']"]
        .attributes["lastBuildStatus"]

  activity = doc.elements["//Project[@name='sepm']"]
        .attributes["activity"]

  build_name = doc.elements["//Project[@name='sepm']"]
        .attributes["lastBuildLabel"]


  if activity == "Sleeping"
    if status == 'Success'
      status = 'ok'
      activity = "Succeeded"
    else
      status = 'warning'
      activity = "Failed"
    end
    activity = "\##{build_name} #{activity}"
  else
    status = "building"
    activity = "\##{Integer(build_name) + 1} #{activity}"
  end


  send_event('build_status', { text: activity, status: status })

end # SCHEDULER
