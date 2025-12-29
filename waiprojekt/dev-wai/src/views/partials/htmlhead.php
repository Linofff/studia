<!doctype html>
<html lang="pl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <title><?= $title ?></title>
    <style>
        * {
            background-color: #1f1f1f;
            color: white;
        }
        .photos {
            display: flex;
            flex-wrap: wrap;
            gap: 10px; /* Space between photos */
        }

        .photo-card {
            border: 1px solid white; /* Or black/gray if background is white */
            padding: 10px;
            /* Optional: prevents cards from becoming huge */
            max-width: 250px;
        }
        .submit_button {
            margin-top: 5px;
            padding-top: 3px;
        }
        .page_links a {
            border: solid 1px white;
            padding: 3px;
            text-decoration: none;
        }
        .page_links {
            margin-top: 10px;
        }

        .form_class input {
            margin-top: 7px;
        }
    </style>
</head>
<body>
<?php require "htmlnav.php"?>
<br>
