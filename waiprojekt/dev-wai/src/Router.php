<?php

class Router {
    private $routes = [];

    public function AddRoute($uri, $controller, $action, $method): void {
        $this->routes[] = [
            'uri' => $uri,
            'controller' => $controller,
            'action' => $action,
            'method' => $method
        ];
    }

    public function Dispatch($uri, $method): void {
        foreach ($this->routes as $route) {
            if ($route['uri'] === $uri && $route['method'] === $method) {
                require __DIR__ . "/controllers/" . $route['controller'] . ".php";
                $controllerClass = $route['controller'] . "Controller";
                $controller = new $controllerClass();
                $controller->{$route['action']}();
                return;
            }
        }
        http_response_code(404);
        echo "404 Not Found";
    }
}
