<!DOCTYPE html>
<html>
    <head>
        <title>PHP Test</title>
    </head>
    <body>
        <?php echo '<h1>Hello World from PHP</h1>'; ?>
		<div id="phpinfo">
		<?php
		ob_start();
		phpinfo();
		$info = ob_get_clean();

		$lines = explode("\n", $info);

		echo "<ul>";
		$sectionOpen = false;

		foreach($lines as $line) {
			if (strpos($line, '=>') !== false) {
				list($key, $value) = explode('=>', $line);
				echo "<li>" . trim($key) . " : " . trim($value) . "</li>";
			} elseif (trim($line)) {
				if ($sectionOpen) {
					echo "</ul>";
				}
				echo "<hr><li><strong>" . trim($line) . "</strong><ul>";
				$sectionOpen = true;
			}
		}

		if ($sectionOpen) {
			echo "</ul>";
		}
		echo "</li></ul>";
		?>

		</div>

    </body>
</html>
