SELECT COUNT(*) AS '타입별로 잡힌 포켓몬 수'
FROM CatchedPokemon, Pokemon AS P
WHERE pid = P.id
GROUP BY type
ORDER BY type;