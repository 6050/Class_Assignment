SELECT name AS '어느 트레이너에게도 잡히지 않은 포켓몬의 이름'
FROM Pokemon
WHERE id NOT IN (SELECT pid FROM CatchedPokemon)
ORDER BY name;