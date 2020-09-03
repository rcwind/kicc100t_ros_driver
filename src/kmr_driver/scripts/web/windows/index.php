<html>
  <head>
    <title>Kmr Firmware</title>
  </head>
  <body>
  <?php
    //path to directory to scan
    $directory = "./";
 
    //get all image files with a .hex extension.
    $sdk_bundles = glob($directory . "*.zip");
 
    //print each file name
    echo("<h1>Windows Drivers for Kmr</h1>");
    echo("<p>The following archives are sdk bundles for the kmr driver. They include ");
    echo("headers and libraries for kmr and all of its dependencies. Refer to the ");
    echo("<a href='http://kmr.yujinrobot.com/documentation/howtos/upgrading-firmware/'>howto</a> ");
    echo("for installation instructions.");
    echo("<ul>\n");
    echo("  <li><a href='./MD5SUMS'>MD5SUMS</a></li>\n");
    // show links
    foreach($sdk_bundles as $sdk)
    {
      $name = substr(${sdk},2);
      if ( is_link(${sdk}) ) {
        $link_name = substr(readlink(${sdk}),11,-13);
        echo("  <li><a href='./${sdk}'>${name}</a> -> ${link_name}</li>\n");
      }
    }
    // show real files
    foreach($sdk_bundles as $sdk)
    {
      $name = substr(${sdk},2);
      if ( !is_link(${sdk}) ) {
        echo("  <li><a href='./${sdk}'>${name}</a></li>\n");
      }
    }
    echo("</ul>\n");
    echo("<p>Some history (look for items linked to <i>kmr_driver)</i>:</p>");
    echo("<ul>\n");
    echo("  <li><a href='http://ros.org/wiki/kmr/ChangeList'>Changelog</a></li>\n");
    echo("</ul>\n");
  ?>
  </body>
</html>

