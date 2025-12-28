<!doctype html>
<html lang="pl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>UploadPhoto</title>
</head>
<body>
<h2>Upload Photo</h2>
<form action="/" method="POST" enctype="multipart/form-data">
    <label>
        Chose photo to upload
        <input type="file" name="file">
    </label>
    <input type="submit" value="upload">
</form>
</body>
</html>